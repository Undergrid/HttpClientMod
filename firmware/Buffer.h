#pragma once

 class Buffer {
     public:
        virtual bool append(const char c);
        virtual void clear();
        bool isFull() { return full; }
        virtual const char* const getBuffer() const;
    
    protected:
        bool full = false;
 };
 
 template<int LEN> 
 class StaticBuffer : public Buffer {
    public:
        StaticBuffer() {
            clear();
        }
        
        bool append(const char c) {
            if (full) return false;
            buffer[offset] = c;
            offset += 1;
            if (offset == LEN) {
                buffer[LEN-1] = '\0';
                full = true;
                return false;
            }
            return true;
        }
        
        void clear() {
            full = false;
            offset = 0;
            memset(buffer, 0, sizeof(char) * LEN);
        }
        
        bool isFull() const {
            return full;
        }
        
        const char* const getBuffer() const {
            return buffer;
        }

    private:
        int offset;
        char buffer[LEN];     
 };
 
 class DynamicBuffer : public Buffer {
     public:
        DynamicBuffer(int initialLen=1024, int increment=256, int maxLen=2048);
        ~DynamicBuffer();
        
        bool append(const char c);
        void clear();
        const char* const getBuffer() const;
        
    private:
        int len;
        int inc;
        int max;
        int currentLen;
        int offset;
        char *buffer;
        
        bool resizeBuffer(void);
 };