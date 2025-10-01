#pragma once

#include <QtGlobal>
#include <QDebug>

/**
 * @brief Track/Sector pair structure for DOS 3.3 disk operations
 * 
 * Represents a track and sector coordinate on a DOS 3.3 disk.
 * Used throughout the DOS 3.3 file system implementation for
 * addressing sectors and managing disk layout.
 */
class TSPair final
{
public:
    /**
     * @brief Default constructor - creates invalid (0,0) TSPair
     */
    constexpr TSPair() noexcept : m_track(0), m_sector(0) {}
    
    /**
     * @brief Construct TSPair with specific track and sector
     * @param trackval Track number (0-34, or 0xFF for invalid)
     * @param secval Sector number (0-15)
     */
    constexpr TSPair(quint8 trackval, quint8 secval) noexcept 
        : m_track(trackval), m_sector(secval) {}

    /**
     * @brief Set the track number
     * @param tracknum Track number (0-34, or 0xFF for invalid)
     */
    void setTrack(quint8 tracknum) noexcept;

    /**
     * @brief Set the sector number  
     * @param secnum Sector number (0-15)
     */
    void setSector(quint8 secnum) noexcept;

    /**
     * @brief Check if this TSPair represents a valid disk location
     * @return true if track < 35 (and not 0xFF) and sector < 16
     */
    [[nodiscard]] bool isValid() const noexcept;

    /**
     * @brief Get the track number
     * @return Track number
     */
    [[nodiscard]] constexpr quint8 track() const noexcept { return m_track; }
    
    /**
     * @brief Get the sector number
     * @return Sector number  
     */
    [[nodiscard]] constexpr quint8 sector() const noexcept { return m_sector; }

    /**
     * @brief Equality comparison operator
     * @param other TSPair to compare against
     * @return true if both track and sector match
     */
    [[nodiscard]] constexpr bool operator==(const TSPair& other) const noexcept
    {
        return other.track() == track() && other.sector() == sector();
    }

    /**
     * @brief Inequality comparison operator
     * @param other TSPair to compare against  
     * @return true if track or sector differ
     */
    [[nodiscard]] constexpr bool operator!=(const TSPair& other) const noexcept
    {
        return !operator==(other);
    }

    /**
     * @brief Debug output helper
     */
    void dump() const;

private:
    quint8 m_track;
    quint8 m_sector;
};