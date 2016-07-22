#include "application.h"
#include "Buffer.h"

    DynamicBuffer::DynamicBuffer(int initialLen, int increment, int maxLen) {
        len = initialLen;
        inc = increment;
        max = maxLen;
        
        buffer = NULL;
        clear();
    }
    
    DynamicBuffer::~DynamicBuffer() {
        free(buffer);
    }
        
    bool DynamicBuffer::append(const char c) {
        if (full) return false;
        buffer[offset] = c;
        offset++;
        if ((offset == currentLen) && (!resizeBuffer())) {
            buffer[currentLen-1] = '\0';
            full = true;
            return false;
        }
        return true;
    }
    
    void DynamicBuffer::clear() {
        if (buffer != NULL) {
            free(buffer);
        }
        
        full = false;
        offset = 0;
        currentLen = len;
        buffer = (char *)malloc(sizeof(char) * len);
        memset(buffer, 0, sizeof(char) * len);
    }
    
    const char* const DynamicBuffer::getBuffer() const {
        return buffer;
    }

    bool DynamicBuffer::resizeBuffer(void) {
        if ((currentLen + inc) > max) return false;
        
        currentLen += inc;
        char *ptr = (char *)realloc(buffer, sizeof(char) * currentLen);
        if (ptr != NULL) {
            buffer = ptr;
            return true;
        } else {
            return false;
        }
    }