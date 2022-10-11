/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Joris Guisson <joris.guisson@gmail.com>                               *
 *   Ivan Vasic <ivasic@gmail.com>                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#ifndef BTTORRENTFILE_H
#define BTTORRENTFILE_H

#include <interfaces/torrentfileinterface.h>
#include <qstring.h>
#include <util/constants.h>

namespace bt
{
class ChunkManager;
class Torrent;

/**
 * @author Joris Guisson
 *
 * File in a multi file torrent. Keeps track of the path of the file,
 * it's size, offset into the cache and between which chunks it lies.
 */
class KTORRENT_EXPORT TorrentFile : public TorrentFileInterface
{
    Q_OBJECT

    Torrent *tor;
    Uint64 cache_offset;
    Priority priority;
    Priority old_priority;
    bool missing;

public:
    /**
     * Default constructor. Creates a null TorrentFile.
     */
    TorrentFile(Torrent *tor = nullptr);

    /**
     * Constructor.
     * @param index Index number of the file
     * @param path Path of the file
     * @param off Offset into the torrent
     * (i.e. how many bytes were all the previous files in the torrent combined)
     * @param size Size of the file
     * @param chunk_size Size of each chunk
     */
    TorrentFile(Torrent *tor, Uint32 index, const QString &path, Uint64 off, Uint64 size, Uint64 chunk_size);

    /**
     * Copy constructor.
     * @param tf The TorrentFile to copy
     */
    TorrentFile(const TorrentFile &tf);
    ~TorrentFile() override;

    /// Get the offset into the torrent
    Uint64 getCacheOffset() const
    {
        return cache_offset;
    }

    /// Get the offset at which the file starts in the first chunk
    Uint64 getFirstChunkOffset() const
    {
        return first_chunk_off;
    }

    /// Get how many bytes the files takes up of the last chunk
    Uint64 getLastChunkSize() const
    {
        return last_chunk_size;
    }

    /// Check if this file doesn't have to be downloaded
    bool doNotDownload() const override
    {
        return (priority == EXCLUDED);
    }

    /// Set whether we have to not download this file
    void setDoNotDownload(bool dnd) override;

    /// Checks if this file is multimedial
    bool isMultimedia() const override;

    /// Gets the priority of the file
    Priority getPriority() const override
    {
        return priority;
    }

    /// Sets the priority of the file
    void setPriority(Priority newpriority = NORMAL_PRIORITY) override;

    /// Get the previous priority value
    Priority getOldPriority() const
    {
        return old_priority;
    }

    /// emits signal.
    void emitDownloadStatusChanged() override;

    void setEmitDownloadStatusChanged(bool show) override
    {
        emit_status_changed = show;
    }

    /**
     * Assignment operator
     * @param tf The file to copy
     * @return *this
     */
    TorrentFile &operator=(const TorrentFile &tf);

    /// See if the file is missing
    bool isMissing() const
    {
        return missing;
    }

    /// Set the file to be missing or not
    void setMissing(bool m)
    {
        missing = m;
    }

    /**
     * Calculate the offset of a chunk in the file
     * @param cindex Index of chunk
     * @param chunk_size Size of each chunk
     */
    Uint64 fileOffset(Uint32 cindex, Uint64 chunk_size) const;

    static TorrentFile null;

    /**
     * Update the number of downloaded chunks for this file.
     * @param cman The ChunkManager
     */
    void updateNumDownloadedChunks(ChunkManager &cman);
};

}

#endif
