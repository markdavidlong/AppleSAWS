/**
 * @file CharSetCharacter.h
 * @brief Individual character definition for Apple II character sets
 * 
 * This file contains the CharSetCharacter class which represents a single
 * character in an Apple II character set with its ASCII value and bitmap data.
 * 
 * @author AppleSAWS Development Team
 * @date 2024
 */

#ifndef CHARSETCHARACTER_H
#define CHARSETCHARACTER_H

#include <QByteArray>
#include <QtGlobal>

/**
 * @class CharSetCharacter
 * @brief Represents a single character in an Apple II character set
 * 
 * Each CharSetCharacter contains an ASCII value and 8 bytes of bitmap data
 * that define how the character appears on screen. The bitmap data represents
 * an 8x8 pixel character where each byte defines one row of pixels.
 */
class CharSetCharacter final
{
public:
    /**
     * @brief Construct character with ASCII value and bitmap data
     * @param asciival ASCII value for this character (default: 0)
     * @param bytes Bitmap data for character display (default: empty)
     */
    explicit CharSetCharacter(quint8 asciival = 0, QByteArray bytes = QByteArray()) noexcept;

    /**
     * @brief Destructor
     */
    ~CharSetCharacter() = default;

    // Rule of 5: Copy operations
    CharSetCharacter(const CharSetCharacter&) = default;
    CharSetCharacter& operator=(const CharSetCharacter&) = default;

    // Rule of 5: Move operations
    CharSetCharacter(CharSetCharacter&&) noexcept = default;
    CharSetCharacter& operator=(CharSetCharacter&&) noexcept = default;

    /**
     * @brief Set the ASCII value for this character
     * @param val ASCII value to set
     */
    void setAsciiVal(quint8 val) noexcept { m_asciival = val; }

    /**
     * @brief Get the ASCII value for this character
     * @return ASCII value of this character
     */
    [[nodiscard]] quint8 asciiVal() const noexcept { return m_asciival; }

    /**
     * @brief Set character bitmap data from byte array
     * @param bytes Bitmap data (will be resized to 8 bytes if needed)
     */
    void setData(QByteArray bytes);

    /**
     * @brief Set character bitmap data from individual bytes
     * @param b0 Row 0 bitmap data (default: 0)
     * @param b1 Row 1 bitmap data (default: 0)
     * @param b2 Row 2 bitmap data (default: 0)
     * @param b3 Row 3 bitmap data (default: 0)
     * @param b4 Row 4 bitmap data (default: 0)
     * @param b5 Row 5 bitmap data (default: 0)
     * @param b6 Row 6 bitmap data (default: 0)
     * @param b7 Row 7 bitmap data (default: 0)
     */
    void setData(quint8 b0 = 0, quint8 b1 = 0, quint8 b2 = 0, quint8 b3 = 0,
                 quint8 b4 = 0, quint8 b5 = 0, quint8 b6 = 0, quint8 b7 = 0);

    /**
     * @brief Get the character bitmap data
     * @return 8-byte array containing character bitmap data
     */
    [[nodiscard]] QByteArray data() const noexcept { return m_data; }

private:
    /// ASCII value for this character
    quint8 m_asciival{0};
    
    /// 8-byte bitmap data defining character appearance
    QByteArray m_data;
};

#endif // CHARSETCHARACTER_H