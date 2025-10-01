/**
 * @file CharacterSet.h
 * @brief Character set handling for Apple II character display
 * 
 * This file contains the CharacterSet class which manages collections of
 * CharSetCharacter objects for Apple II character set display.
 * 
 * @author AppleSAWS Development Team
 * @date 2024
 */

#ifndef CHARACTERSET_H
#define CHARACTERSET_H

#include "CharSetCharacter.h"

#include <QByteArray>
#include <QList>
#include <QMap>

/**
 * @class CharacterSet
 * @brief Manages a collection of Apple II character set characters
 * 
 * The CharacterSet class provides functionality to manage and access
 * character definitions for Apple II character sets, including building
 * character sets from binary data and providing access to individual
 * characters.
 */
class CharacterSet final
{
public:
    /**
     * @brief Default constructor
     */
    CharacterSet() = default;

    /**
     * @brief Destructor
     */
    ~CharacterSet() = default;

    // Rule of 5: Copy operations
    CharacterSet(const CharacterSet&) = default;
    CharacterSet& operator=(const CharacterSet&) = default;

    // Rule of 5: Move operations
    CharacterSet(CharacterSet&&) noexcept = default;
    CharacterSet& operator=(CharacterSet&&) noexcept = default;

    /**
     * @brief Build character set from binary data blob
     * @param data Binary data containing character definitions (expected size: 768 bytes)
     * 
     * The data should contain 96 characters (ASCII 32-127) with 8 bytes per character.
     * If the data size is not 768 bytes, it will be resized and a warning issued.
     */
    void buildSetFromSetBlob(QByteArray data);

    /**
     * @brief Access character by ASCII value
     * @param asciival The ASCII value of the character
     * @return The CharSetCharacter for the given ASCII value
     */
    [[nodiscard]] CharSetCharacter operator[](int asciival) const { return m_charmap[asciival]; }

    /**
     * @brief Get all characters in the set
     * @return List of all CharSetCharacter objects in the set
     */
    [[nodiscard]] QList<CharSetCharacter> allCharacters() const;

    /**
     * @brief Check if character exists in the set
     * @param asciival The ASCII value to check
     * @return true if character exists, false otherwise
     */
    [[nodiscard]] bool contains(int asciival) const noexcept { return m_charmap.contains(asciival); }

private:
    /// Map of ASCII values to character definitions
    QMap<int, CharSetCharacter> m_charmap;
};

#endif // CHARACTERSET_H




