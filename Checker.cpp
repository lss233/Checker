/*========== Programed By chy-2003 ==========*/


#include <cstdio>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include "CheckerLib\CheckerConfigure.h" //for configure
#include <unistd.h>
#define stat _stat
#endif
#ifdef __linux__
#include "CheckerLib/CheckerConfigure.h"
#endif

std::string Construction;
inline int Work1();

int main() {
#ifdef __linux__
	system("mkdir CheckerFiles\n");
#endif
    printf("Checker Exit With Code : %d\n", Work1());
    return 0;
}

inline void Pt(char *S, int T);
bool HasCache(std::string Path)
{
    Path.pop_back();
    struct stat Result;
    if(stat(Path.data(), &Result) != 0) {
        exit(-1);
    }
    time_t ModifyTime = Result.st_mtime;
    auto LastCompiledfile = ".lastcompiled." + Path;
    FILE * fp = fopen(LastCompiledfile.data(), "rb");
    if(fp == NULL)
        return false;
    time_t * LastCompiled;
    LastCompiled = (time_t *) malloc(sizeof(time_t));
    fread(LastCompiled, sizeof(time_t), 1, fp);
    fclose(fp);
    return *LastCompiled == ModifyTime;
}
void SaveCache(std::string Path) {
    Path.pop_back();
    struct stat Result;
    stat(Path.c_str(), &Result);
    time_t ModifyTime = Result.st_mtime;
    auto LastCompiledfile = ".lastcompiled." + Path;
    FILE * fp = fopen(LastCompiledfile.data(), "wb");
    fwrite(&ModifyTime, sizeof(time_t), 1, fp);
    fclose(fp);
}
int Work1()
{
    printf("\033[33mCompiling 4 files, it may takes several seconds. Please wait...\033[0m\n");

    if(HasCache(My)){
        printf("    \033[36mMain.cpp Compile skipped.\033[0m\n");
    } else {
        Construction = Builder + My + "-o " + MyRunfile + BuilderConfigure;
        if (!system(Construction.data())) printf("    \033[32mMain.cpp Compiled.\033[0m\n");
        else return 0, printf("    \033[31mMain.cpp Compile failed.\033[0m\n");
        SaveCache(My);
    }

    if(HasCache(Std)){
        printf("    \033[36mStd.cpp  Compile skipped.\033[0m\n");
    } else {
        Construction = Builder + Std + "-o " + StdRunfile + BuilderConfigure;
        if (!system(Construction.data())) printf("    \033[32mStd.cpp  Compiled.\033[0m\n");
        else return 0, printf("    \033[31mStd.cpp  Compile failed.\033[0m\n");
        SaveCache(Std);
    }

    if(HasCache(DataMaker)){
        printf("    \033[36mDmk.cpp  Compile skipped.\033[0m\n");
    } else {
        Construction = Builder + DataMaker + "-o " + DataMakerRunfile + BuilderConfigure;
        if (!system(Construction.data())) printf("    \033[32mDmk.cpp  Compiled.\033[0m\n");
        else return 0, printf("    \033[31mDmk.cpp  Compile failed.\033[0m\n");
        SaveCache(DataMaker);
    }

    Construction = Builder + Checker + "-o " + CheckerRunfile + BuilderConfigure;
    if (!system(Construction.data())) printf("    \033[32m Judger  Compiled.\033[0m\n");
    else return 0, printf("    \033[31m Judger  Compile failed.\033[0m\n");

    printf("\033[33mRunning...\033[0m\n");
    fflush(stdout);
    int TestCases = 0;
    long long Time1 = 0, Time2 = 0, Time;
    std::chrono::milliseconds Clock1, Clock2;
    while (true) {
        printf("\033[33mCase %5d:\033[0m", ++TestCases);
        fflush(stdout);
        Construction = DataMakerRunfile + "> " + Input;
#ifdef __linux__
        Construction = (std::string)"./" + Construction;
#endif
        if (!system(Construction.data())) printf("\033[33m -> \033[0m");
        else {
            printf("\033[31mdmk failed.\033[0m\n");
            break;
        }
        fflush(stdout);

        Construction = MyRunfile + "< " + Input + "> " + MyOut;
#ifdef __linux__
        Construction = (std::string)"./" + Construction;
#endif
        Clock1 = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch()
			);
        if (!system(Construction.data())) {
            Clock2 = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
                    );
            Time = (Clock2 - Clock1).count();
            Time1 += Time;
            printf("\033[32mMy : [\033[0m");
            Pt((char *)"Cur:", Time);
            printf("\033[32m|\033[0m");
            Pt((char *)"Avr:", Time1 / TestCases);
            printf("\033[32m]  \033[0m");
        } else {
            printf("\033[31mMy : Runtime Error.\033[0m\n");
            break;
        }
        fflush(stdout);

        Construction = StdRunfile + "< " + Input + "> " + StdOut;
#ifdef __linux__
        Construction = (std::string)"./" + Construction;
#endif
        Clock1 = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch()
			);
        if (!system(Construction.data())) {
            Clock2 = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
                    );
            Time = (Clock2 - Clock1).count();
            Time2 += Time;
            printf("\033[32mstd : [\033[0m");
            Pt((char *)"Cur:", Time);
            printf("\033[32m|\033[0m");
            Pt((char *)"Avr:", Time2 / TestCases);
            printf("\033[32m]  \033[0m");
        } else {
            printf("\033[31mstd : Runtime Error.\033[0m\n");
            break;
        }
        fflush(stdout);

        Construction = CheckerRunfile + Input + MyOut + StdOut;
        int ExitCode;
        if (!(ExitCode = system(Construction.data()))) {
        	fflush(stdout);
        } else {
        	fflush(stdout);
            Construction = WayShowDiff + MyOut + StdOut;
            system(Construction.data());
            break;
        }
    }
    return 0;
}

inline void Pt(char *Ch, int Time)
{
    if (Time <= 1000) printf("\033[32m");
    if (Time > 1000 && Time <= 3000) printf("\033[34m");
    if (Time > 3000 && Time <= 5000) printf("\033[33m");
    if (Time > 5000) printf("\033[31m");
    printf("%s%5dms", Ch, Time);
    printf("\033[0m");
    return;
}

