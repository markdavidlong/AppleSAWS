#include "IntBasicFile.h"
#include <QDebug>

IntBasicFile::IntBasicFile(QByteArray data)
{
    setData(data);

    qDebug()<<detokenize();

    setData(detokenize());
}

QByteArray IntBasicFile::detokenize()
{

//    uint  =  unsigned int
//    quint8    =  unsigned char
//    ==========================================================================
    return dumpBufferAsIntBasicFile(data());
}

quint16 IntBasicFile::get16(quint8 v1, quint8 v2)
{
    return (quint16) v1 + ((quint16) v2 * 256);
}

QByteArray IntBasicFile::dumpBufferAsIntBasicFile(QByteArray origdata)
/*
 * THIS CODE IS MODIFIED FROM PAUL SCHYLTER'S SAMPLE CODE AVAILABLE AT:
 * https://macgui.com/usenet/?group=1&start=14720&id=184603
 *
 *
 *   Integer Basic file format:
 *
 *   <Length_of_file> (16-bit little endian)
 *   <Line>
 *   ......
 *   <Line>
 *
 *   where <Line> is:
 *      1 byte:   Line length
 *      2 bytes:  Line number, binary little endian
 *      <token>
 *      <token>
 *      <token>
 *      .......
 *      <end-of-line token>
 *
 *   <token> is one of:
 *      $12 - $7F:   Tokens as listed below: 1 byte/token
 *      $80 - $FF:   ASCII characters with high bit set
 *      $B0 - $B9:   Integer constant, 3 bytes:  $B0-$B9,
 *                     followed by the integer value in
 *                     2-byte binary little-endian format
 *                     (Note: a $B0-$B9 byte preceded by an
 *                      alphanumeric ASCII(hi_bit_set) byte
 *                      is not the start of an integer
 *                      constant, but instead part of a
 *                      variable name)
 *
 *   <end-of-line token> is:
 *      $01:         One byte having the value $01
 *                   (Note: a $01 byte may also appear
 *                    inside an integer constant)
 *
 *  Note that the tokens $02 to $11 represent commands which
 *  can be executed as direct commands only -- any attempt to
 *  enter then into an Integer Basic program will be rejected
 *  as a syntax error.  Therefore, no Integer Basic program
 *  which was entered through the Integer Basic interpreter
 *  will contain any of the tokens $02 to $11.  The token $00
 *  appears to be unused and won't appear in Integer Basic
 *  programs either.  However, $00 is used as an end-of-line
 *  marker in S-C Assembler source files, which also are of
 *  DOS file type "I".
 *
 *  (note here a difference from Applesoft Basic, where there
 *  are no "direct mode only" commands - any Applesoft commands
 *  can be entered into an Applesoft program as well).
 *
 */
{
#define REM_TOKEN   0x5D
#define UNARY_PLUS  0x35
#define UNARY_MINUS 0x36
#define QUOTE_START 0x28
#define QUOTE_END   0x29
    static const char *itoken[128] =
    {
        /* $00-$0F */
        "HIMEM:","", "_",     " : ",
        "LOAD",  "SAVE",  "CON",   "RUN",    /* Direct commands */
        "RUN",   "DEL",   ",",     "NEW",
        "CLR",   "AUTO",  ",",     "MAN",

        /* $10-$1F */
        "HIMEM: ","LOMEM: "," + ",     " - ",     /* Binary ops */
        " * ",     " / ",     " = ",     " # ",
        " >= ",    " > ",     " <= ",    " <> ",
        " < ",     " AND ",   " OR ",    " MOD ",

        /* $20-$2F */
        " ^",     "+",     " (",     " , ",
        " THEN ",  " THEN ",  " , ",     " , ",
        "\"",    "\"",    " (",      " ! ",
        " ! ",     " (",     " PEEK ",  " RND ",

        /* $30-$3F */
        " SGN ",   " ABS ",   " PDL ",   " RNDX ",
        " (",     " + ",     " - ",     " NOT ",      /* Unary ops */
        " (",     " = ",     " # ",     " LEN(",
        " ASC(",  " SCRN(", " , ",     " (",

        /* $40-$4F */
        " $",     " $",     " (",     " , ",
        " , ",     " ; ",     " ; ",     " ; ",
        " , ",     " , ",     " , ",     " TEXT ",  /* Statements */
        " GR ",    " CALL ",  " DIM ",   " DIM ",

        /* $50-$5F */
        " TAB ",   " END ",   " INPUT ", " INPUT ",
        " INPUT ", " FOR ",   " = ",     " TO ",
        " STEP ",  " NEXT ",  " , ",     " RETURN ",
        " GOSUB ", " REM ",   " LET ",   " GOTO ",

        /* $60-$6F */
        " IF ",    " PRINT ", " PRINT ", " PRINT ",
        " POKE ",  " , ",     " COLOR= "," PLOT ",
        " , ",     " HLIN ",  " , ",     " AT ",
        " VLIN ",  " , ",     " AT ",    " VTAB ",

        /* $70-$7F */
        " = ",     " = ",     ") ",     ") ",
        " LIST ",  " , ",     " LIST ",  " POP ",
        " NODSP ", " DSP ",  " NOTRACE "," DSP ",
        " DSP ",   " TRACE ", " PR#" ,   " IN# ",
    };

    QByteArray retval;

    QVector<quint8> data;
    QByteArray data0 = origdata;


    foreach (quint8 value, origdata)
    {
        data.append(value);
    }
    data.removeFirst();

    int alen = get16(data[0],data[1]);

    int position = 2;

    while (/*position <= alen-2 &&*/ position < data.length())// && data[position])
    {
        bool inREM = false;
        bool inQUOTE = false;
        int lastAN = 0;
        int leadSP = 0;
        int lastTOK = 0;

        unsigned int lineno;
        lineno = get16(data[position],data[position+1]);
        position += 2;
        //fprintf( fptr, "%u ", lineno );
        retval.append(QString::asprintf("%u ", lineno ).toLocal8Bit());

        for( ;  position < data.length() && data[position]!=0x01  ; position++)
        {
            leadSP = leadSP || lastAN;
            if ( data[position] & 0x80 )
            {
                if ( !inREM && !inQUOTE && !lastAN &&
                     (data[position]>=0xb0 && data[position]<=0xb9) )
                {
                    qint16 integerval = get16(data[position+1],data[position+2]);
                    int leadspace = lastTOK && leadSP;
                    retval.append(QString::asprintf(leadspace ? " %d" : "%d", (int) integerval ));
                    position += 2;
                    leadSP = 1;
                }
                else
                {
                    quint8 c = data[position] & 0x7F;
                    int leadspace = !inREM && !inQUOTE &&
                                    lastTOK && leadSP && isalnum(c);
                    if ( leadspace )
                        retval.append(QString::asprintf(" "));

                    if ( c >= 0x20 )
                        retval.append(QString::asprintf("%c", c ));
                    else
                        retval.append(QString::asprintf("^%c", c+0x40 ));
                    lastAN = isalnum(c);
                }
                lastTOK = 0;
            }
            else
            {
                const char *tok = itoken[data.at(position)];
                char lastchar = tok[strlen(tok)-1];
                int leadspace = leadSP &&
                          ( isalnum(tok[0]) ||
                            data[position] == UNARY_PLUS ||
                            data[position] == UNARY_MINUS ||
                            data[position] == QUOTE_START  );
                switch( data[position] )
                {
                    case REM_TOKEN:   inREM = true;    break;
                    case QUOTE_START: inQUOTE = true;  break;
                    case QUOTE_END:   inQUOTE = false;  break;
                    default:  break;
                }
                retval.append(QString::asprintf(leadspace ? " %s" : "%s", tok ));
                lastAN  = 0;
                leadSP = isalnum(lastchar) || lastchar == ')' || lastchar == '\"';
                lastTOK = 1;
            }
        }
        retval.append("\n");
        position++;
        position++;
//           if ( pause(0,f) < 0 )
//               goto exit;
    }
   // len = len;
//  exit:
    return retval;
}  /* dumpBufferAsIntBasicFile */


//int origdumpBufferAsIntBasicFile( quint8 *data, char *fname, uint len, FILE *fptr )
///*
// *   Integer Basic file format:
// *
// *   <Length_of_file> (16-bit little endian)
// *   <Line>
// *   ......
// *   <Line>
// *
// *   where <Line> is:
// *      1 byte:   Line length
// *      2 bytes:  Line number, binary little endian
// *      <token>
// *      <token>
// *      <token>
// *      .......
// *      <end-of-line token>
// *
// *   <token> is one of:
// *      $12 - $7F:   Tokens as listed below: 1 byte/token
// *      $80 - $FF:   ASCII characters with high bit set
// *      $B0 - $B9:   Integer constant, 3 bytes:  $B0-$B9,
// *                     followed by the integer value in
// *                     2-byte binary little-endian format
// *                     (Note: a $B0-$B9 byte preceded by an
// *                      alphanumeric ASCII(hi_bit_set) byte
// *                      is not the start of an integer
// *                      constant, but instead part of a
// *                      variable name)
// *
// *   <end-of-line token> is:
// *      $01:         One byte having the value $01
// *                   (Note: a $01 byte may also appear
// *                    inside an integer constant)
// *
// *  Note that the tokens $02 to $11 represent commands which
// *  can be executed as direct commands only -- any attempt to
// *  enter then into an Integer Basic program will be rejected
// *  as a syntax error.  Therefore, no Integer Basic program
// *  which was entered through the Integer Basic interpreter
// *  will contain any of the tokens $02 to $11.  The token $00
// *  appears to be unused and won't appear in Integer Basic
// *  programs either.  However, $00 is used as an end-of-line
// *  marker in S-C Assembler source files, which also are of
// *  DOS file type "I".
// *
// *  (note here a difference from Applesoft Basic, where there
// *  are no "direct mode only" commands - any Applesoft commands
// *  can be entered into an Applesoft program as well).
// *
// */
//{
//#define REM_TOKEN   0x5D
//#define UNARY_PLUS  0x35
//#define UNARY_MINUS 0x36
//#define QUOTE_START 0x28
//#define QUOTE_END   0x29
//    static const char *itoken[128] =
//    {
//        /* $00-$0F */
//        "HIMEM:","<$01>", "_",     " : ",
//        "LOAD",  "SAVE",  "CON",   "RUN",    /* Direct commands */
//        "RUN",   "DEL",   ",",     "NEW",
//        "CLR",   "AUTO",  ",",     "MAN",

//        /* $10-$1F */
//        "HIMEM:","LOMEM:","+",     "-",     /* Binary ops */
//        "*",     "/",     "=",     "#",
//        ">=",    ">",     "<=",    "<>",
//        "<",     "AND",   "OR",    "MOD",

//        /* $20-$2F */
//        "^",     "+",     "(",     ",",
//        "THEN",  "THEN",  ",",     ",",
//        "\"",    "\"",    "(",     "!",
//        "!",     "(",     "PEEK",  "RND",

//        /* $30-$3F */
//        "SGN",   "ABS",   "PDL",   "RNDX",
//        "(",     "+",     "-",     "NOT",      /* Unary ops */
//        "(",     "=",     "#",     "LEN(",
//        "ASC(",  "SCRN(", ",",     "(",

//        /* $40-$4F */
//        "$",     "$",     "(",     ",",
//        ",",     ";",     ";",     ";",
//        ",",     ",",     ",",     "TEXT",  /* Statements */
//        "GR",    "CALL",  "DIM",   "DIM",

//        /* $50-$5F */
//        "TAB",   "END",   "INPUT", "INPUT",
//        "INPUT", "FOR",   "=",     "TO",
//        "STEP",  "NEXT",  ",",     "RETURN",
//        "GOSUB", "REM",   "LET",   "GOTO",

//        /* $60-$6F */
//        "IF",    "PRINT", "PRINT", "PRINT",
//        "POKE",  ",",     "COLOR=","PLOT",
//        ",",     "HLIN",  ",",     "AT",
//        "VLIN",  ",",     "AT",    "VTAB",

//        /* $70-$7F */
//        "=",     "=",     ")",     ")",
//        "LIST",  ",",     "LIST",  "POP",
//        "NODSP", "DSP",  "NOTRACE","DSP",
//        "DSP",   "TRACE", "PR#",   "IN#",
//    };

//    quint8 *data0 = data;
//    int alen = get16(data);

//    for( data+=2; *data && (data-data0 <= alen); )
//    {
//        bool inREM = false;
//        bool inQUOTE = false;
//        int lastAN = 0;
//        int leadSP = 0;
//        int lastTOK = 0;

//        unsigned int lineno;
////           unsigned int linelen = *data++;
//        lineno = get16(data), data += 2;
//        //linelen = linelen;
//        fprintf( fptr, "%u ", lineno );

//        for( ; *data!=0x01; data++ )
//        {
//            leadSP = leadSP || lastAN;
//            if ( *data & 0x80 )
//            {
//                if ( !inREM && !inQUOTE && !lastAN && (*data >= 0xB0 && *data <= 0xB9) )
//                {
//                    signed short integer = get16(data+1);
//                    int leadspace = lastTOK && leadSP;
//                    fprintf( fptr, leadspace ? " %d" : "%d", (int) integer );
//                    data += 2;
//                    leadSP = 1;
//                }
//                else
//                {
//                    char c = *data & 0x7F;
//                    int leadspace = !inREM && !inQUOTE &&
//                                    lastTOK && leadSP && isalnum(c);
//                    if ( leadspace )
//                        fprintf( fptr, " " );
//                    if ( c >= 0x20 )
//                        fprintf( fptr, "%c", c );
//                    else
//                        fprintf( fptr, "^%c", c+0x40 );
//                    lastAN = isalnum(c);
//                }
//                lastTOK = 0;
//            }
//            else
//            {
//                const char *tok = itoken[*data];
//                char lastchar = tok[strlen(tok)-1];
//                int leadspace = leadSP &&
//                          ( isalnum(tok[0]) ||
//                            *data == UNARY_PLUS ||
//                            *data == UNARY_MINUS ||
//                            *data == QUOTE_START  );
//                switch( *data )
//                {
//                    case REM_TOKEN:   inREM = true;    break;
//                    case QUOTE_START: inQUOTE = true;  break;
//                    case QUOTE_END:   inQUOTE = false;  break;
//                    default:  break;
//                }
//                fprintf( fptr, leadspace ? " %s" : "%s", tok );
//                lastAN  = 0;
//                leadSP = isalnum(lastchar) || lastchar == ')' || lastchar == '\"';
//                lastTOK = 1;
//            }
//        }
//        fprintf( fptr, "\n" ), data++;
////           if ( pause(0,f) < 0 )
////               goto exit;
//    }
//   // len = len;
//    if ( fptr != stdout )
//        fprintf( fptr, "\nSAVE %s\n", fname );
////  exit:
//    return 0;
//}  /* dumpBufferAsIntBasicFile */


