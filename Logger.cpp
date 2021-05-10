#include <iostream>
#include <array>
#include<vector>
#include<fstream>

namespace Logs {
    class FileSink
    {
    public:
        FileSink(const std::string& fine_name, std::size_t buffer_size = 4096u)
            :m_buffer(buffer_size), m_output(fine_name), m_offset(0u)
        {
            if (!m_output.is_open()) {
                throw std::runtime_error("File was not created");
            }
        }

        void Write(const std::string& data)
        {
            const std::size_t data_size = data.size();
            if (data_size + m_offset >= m_buffer.size()) {
                Flush();
            }

            std::copy(begin(data), end(data), begin(m_buffer) + m_offset);
            m_offset += data_size;
        }

        ~FileSink()
        {
            Flush();
        }

    private: 
        void Flush()
        {
            if (m_offset != 0u) {
                m_output.write(m_buffer.data(), m_offset);
                m_offset = 0u;
            }
        }
    private:
        std::vector<char> m_buffer;
        std::ofstream m_output;
        std::size_t m_offset;
    };

    class Logger
    {
    public:
       enum Level
        {
            TRACE_LEVEL,
            DEBUG_LEVEL,
            INFO_LEVEL,
            WARN_LEVEL,
            ERROR_LEVEL,
            FATAL_LEVEL,
        };

         std::array<std::string, Level::FATAL_LEVEL + 1u> LevelStr = {
            "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
        };
    public:
        Logger(const std::string& file_name)
            : m_sink(file_name)
        {

        }

        void Log(Level level, const char* sourse, const std::string& message)
        {
            const std::string formated_message = "[" + LevelStr[level] + "] - " + sourse + " - " + message +"\n";
            m_sink.Write(formated_message);
        }
    private:
        FileSink m_sink;
    };
}

inline Logs::Logger& getFlobalLogger()
{
    static Logs::Logger logger("F:/D/cpp/data.log");
    return logger;
};

#define STR_(x) #x
#define STR(x) STR_(x)

#define LOG_TRACE(message) getFlobalLogger().Log(Logs::Logger::Level::TRACE_LEVEL, __FILE__ ":" STR(__LINE__), (message))
#define LOG_DEBUG(message) getFlobalLogger().Log(Logs::Logger::Level::DEBUG_LEVEL, __FILE__ ":" STR(__LINE__), (message))
#define LOG_INFO(message) getFlobalLogger().Log(Logs::Logger::Level::INFO_LEVEL, __FILE__ ":" STR(__LINE__), (message))
#define LOG_WARN(message) getFlobalLogger().Log(Logs::Logger::Level::WARN_LEVEL, __FILE__ ":" STR(__LINE__), (message))
#define LOG_ERROR(message) getFlobalLogger().Log(Logs::Logger::Level::ERROR_LEVEL, __FILE__ ":" STR(__LINE__), (message))
#define LOG_FATAL(message) getFlobalLogger().Log(Logs::Logger::Level::FATAL_LEVEL, __FILE__ ":" STR(__LINE__), (message))

int main()
{
    for (int i = 0; i < 100; ++i) {
       LOG_DEBUG("Hello from main thread.");
   }
}
