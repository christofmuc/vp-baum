#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring> // for std::memcpy

#include <assert.h>

class Seite
{
private:
    std::vector<uint8_t> seite;
    uint8_t* pos;
    long fileposition = -1;

public:
    Seite(std::size_t groesse)
        : seite(groesse, 0), pos(seite.data()) {}

    Seite(std::size_t groesse, FILE* f, long position)
        : seite(groesse), pos(seite.data())
    {
        std::fseek(f, position, SEEK_SET);
        fileposition = std::ftell(f);
        if (feof(f)) {
            throw std::runtime_error("Can't read at end of file");
        }
        size_t read_size = std::fread(seite.data(), 1, groesse, f);
        if (read_size != groesse) {
            throw std::runtime_error("Failed to read the expected amount from file.");
        }
    }

    // Destructor is not needed as std::vector will automatically release memory

    void* lesen(std::size_t size)
    {
        if (pos + size > seite.data() + seite.size()) {
            throw std::runtime_error("Reading beyond buffer limits.");
        }
        void* current = pos;
        pos += size;
        return current;
    }

    void* schreiben(int32_t inhalt)
    {
        return writeData(&inhalt, sizeof(inhalt));
    }

    void* schreiben(int64_t inhalt)
    {
        return writeData(&inhalt, sizeof(inhalt));
    }

    void* schreiben(float inhalt)
    {
        return writeData(&inhalt, sizeof(inhalt));
    }

    void* schreiben(void* src, std::size_t size)
    {
        return writeData(src, size);
    }

    long anhaengen(FILE* f)
    {
        std::fseek(f, 0, SEEK_END);
        long wrote_position = std::ftell(f);
        size_t wrote_bytes = std::fwrite(seite.data(), 1, seite.size(), f);
        if (std::fwrite(seite.data(), 1, seite.size(), f) != seite.size()) {
            throw std::runtime_error("Failed to write the expected amount to file.");
        }
        printf("Wrote %d bytes to %d\n", wrote_bytes, wrote_position);
        fileposition = wrote_position;
        return wrote_position;
    }

    long speichern(FILE* f, long position)
    {
        std::fseek(f, position, SEEK_SET);
        long wrote_position = std::ftell(f);
        assert(position == wrote_position);
        size_t wrote_bytes = std::fwrite(seite.data(), 1, seite.size(), f);
        if (wrote_bytes != seite.size()) {
            throw std::runtime_error("Failed to write the expected amount to file.");        
        }
        printf("Wrote %d bytes to %d\n", wrote_bytes, wrote_position);
        fileposition = position;
        return position;
    }

private:
    void* writeData(void* data, std::size_t size)
    {
        if (pos + size > seite.data() + seite.size()) {
            throw std::runtime_error("Writing beyond buffer limits.");
        }
        std::memcpy(pos, data, size);
        void* current = pos;
        pos += size;
        return current;
    }
};
