#include "Debug.h"

Log::Log()
{
    isOpened = false;
}

Log::~Log()
{
    file.flush();
    file.close();
}

Log& Log::Get()
{
    if (instance == NULL)
        instance = new Log();
    return *instance;
}

void Log::OpenFile(std::filesystem::path const& filename)
{
    if (isOpened)
        return;

    file.open(filename, std::ios_base::app);
    isOpened = true;
}

void Log::Print(const char* format, ...)
{
    std::string printedText;
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) 
    {
        if (format[i] == '%') 
        {
            i++;
            if (format[i] == 'c') 
            {
                char c = (char)va_arg(args, int);
                printedText.push_back(c);
            }
            else if (format[i] == 's') 
            {
                char* str = va_arg(args, char*);
                for (int j = 0; str[j] != '\0'; j++) 
                {
                    printedText.push_back(str[j]);
                }
            }
            else if (format[i] == 'd') 
            {
                int d = va_arg(args, int);
                std::string str = std::to_string(d);
                printedText.append(str);

            }
            else if (format[i] == 'f') 
            {
                double f = va_arg(args, double);
                std::string str = std::to_string(f);
                printedText.append(str);
            }
            else 
            {
                printedText.push_back('?');
            }
        }
        else 
        {
            printedText.push_back(format[i]);
        }
    }

    va_end(args);
    std::cout << printedText;
    if (isOpened)
    {
        file.write(&printedText[0], printedText.size());
        file.flush();
    }
}

void Log::Destroy()
{
    file.close();
    if (instance)
        delete instance;
}

void DebugLog(std::string file, std::string line, const char* debugMessage, ...)
{
    std::string printedText = file + "(" + line + "): ";
    va_list args;
    va_start(args, debugMessage);

    for (int i = 0; debugMessage[i] != '\0'; i++)
    {
        if (debugMessage[i] == '%')
        {
            i++;
            if (debugMessage[i] == 'c')
            {
                char c = (char)va_arg(args, int);
                printedText.push_back(c);
            }
            else if (debugMessage[i] == 's')
            {
                char* str = va_arg(args, char*);
                for (int j = 0; str[j] != '\0'; j++)
                {
                    printedText.push_back(str[j]);
                }
            }
            else if (debugMessage[i] == 'd')
            {
                int d = va_arg(args, int);
                std::string str = std::to_string(d);
                printedText.append(str);

            }
            else if (debugMessage[i] == 'f')
            {
                double f = va_arg(args, double);
                std::string str = std::to_string(f);
                printedText.append(str);
            }
            else
            {
                printedText.push_back('?');
            }
        }
        else
        {
            printedText.push_back(debugMessage[i]);
        }
    }

    va_end(args);

    OutputDebugStringA(printedText.c_str());
}