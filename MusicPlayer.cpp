// file stream
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <sstream>

using namespace std;

// Can any body help me optimize this Program :(
int ival = 0;
bool menu = true,listloop;
vector<string> list;
string musicpath;
ifstream fin;
int main() {
    system("color 0A");
    vector<string> vec;
    string print,input,dircmd;
    bool JumpAction(int);
    int MenuOperation(int),line = 0;
    void log(string), loadlog(), PlayerScreen(string,string);
    log("User Open Alexzms Music Player");
    menu:
    // open file
    fin.open(".\\musicplayer\\start.txt");
    for ( ; ; ) {
        // clear screen
        system("cls");
        // Load From File And Push It Into vec
        while (getline(fin,print)) {
            vec.push_back(print);
        }
        fin.close();
        // print the arrow
        for (unsigned int x = 0; x != vec.size(); ++x) {
            if (x == (line + 3) && menu) {
                cout << "\t    -> ";
            }
            if (x > 2 && x != (line + 3) && menu) {
                cout << "\t\t";
            }
            cout << vec[x] << '\n';
        }
        if (!menu) {
            break;
        }
        line = MenuOperation(line);
        // operations of arrows + arrow out of bounds
        if (line < 0) {
            if (JumpAction(line)) {
                menu = false;
                vec.clear();
            }
            if (menu) {
                line = 0;
            }
        }
        // arrow out of bounds
        if ((line > static_cast<int>(vec.size() - 4)) && menu) {
            cout << "ArrowOutOfBounds" << '\n';
            line = vec.size() - 4;
        }
    }
    switch (line) {
        case -2:
            getline(cin,input);
            input = "dir " + input + "\\*.mp3 /p/b";
            system(input.c_str());
            log("User: " + input);
            system("pause");
            break;
        case -3:
            getline(cin, musicpath);
            cout << "MusicPath ( " << musicpath << " ) Is Successfully Set.\n";
            log("User: Set Music Path to " + musicpath);
            system("pause");
            break;
        case -4:
            if (musicpath.empty()) {
                cout << "Please Set Muisc Path First!";
                getch();
                break;
            }
            cout << musicpath;
            getline(cin,input);
            vector<string> loadfile(string);
            dircmd = "dir " + musicpath + "\\*.mp3 /p/b > \".\\musicplayer\\temp\\musiclist.txt\"";
            system(dircmd.c_str());
            list = loadfile(".\\musicplayer\\temp\\musiclist.txt");
            PlayerScreen(musicpath,input);
            log("User: " + input + " PlaySound");
            break;
        case -5:
            system("pause");
            break;
        case -6:
            loadlog();
            log("User: LoadLog");
            system("pause");
            break;
        case -7:
            log("User: Exit Alexzms Music Player");
            return 0;
        default:
            ;
    }
    vec.clear();
    cin.clear();
    cin.sync();
    line = 0;
    menu = true;
    goto menu;
    system("pause");
    return 0;
}

// get operations of arrows
int MenuOperation(int line) {
    char action = getch();
    switch (action) {
        case 's':
            PlaySound(TEXT(".\\musicplayer\\sounds\\arrowmove.wav"), NULL, SND_FILENAME | SND_ASYNC );
            return line + 1;
        case 'w':
            PlaySound(TEXT(".\\musicplayer\\sounds\\arrowmove.wav"), NULL, SND_FILENAME | SND_ASYNC );
            return line - 1;
        case 'd':
            return -2 - line;
        default:
            ;
    }
    return line;
}

// operations of arrows
bool JumpAction(int line) {
    if (line > -2) {
        return false;
    }
    stringstream stream;
    string str;
    stream << line;
    stream >> str;
    fin.close();
    fin.open(".\\musicplayer\\line\\" + str + ".txt");
    return true;
}

// write log
void log(string text) {
    ofstream fout;
    string temp,rmspace(string);
    fout.open(".\\musicplayer\\log\\log.txt",ios::app);    
    time_t timeval; 
    timeval = time(NULL); 
    temp = static_cast<string> (ctime(&timeval));
    fout << temp << text << "\r\n";
    fout.close();
}

// read log and print it
// also you can use :
// system("type .\\musicplayer\\log\\log.txt");
void loadlog() {
    ifstream finput(".\\musicplayer\\log\\log.txt");
    string temp;
    while (getline(finput,temp)) {
        cout << temp << '\n';
    }
    finput.close();
}


// musicplayer screen (need optimization)
void PlayerScreen(string path,string name) {
    bool ifPaused = false, ifChange = false, ifResume = true, ifRepeat = true, refresh = false;
    int screenOperation(),line = 3;
    vector<string> vec;
    string temp, para, fullpath = path + name;
    string open = "open \"" + fullpath + "\" alias music", play = "play music", close = "close music";
    repeat: 
    ifstream finput(".\\musicplayer\\screen.txt");
    mciSendString(open.c_str(), NULL, 0, NULL);
    // get the length of music
    char sPosition[256];unsigned long pos,length;
    mciSendString("Status music length", sPosition, 255,0);length=strtol(sPosition, NULL, 10);
    mciSendString(play.c_str(), NULL, 0, NULL);
    if (length == 0 && !listloop) {
        cout << "Error! Music Load Failed!\n\t>>Yes<<\n";
        getch();
        return;
    }
    if (length == 0 && listloop) {
        PlayerScreen(path,list[ival ++]);
    }
    for ( ; ; ) {
        refreshpoint:
        system("cls");
        mciSendString("Status music position", sPosition, 255,0);pos=strtol(sPosition, NULL, 10);
        if (pos < 0) {
            pos = 1;
        }
        if (ifRepeat) {
            if (pos == length) {
                mciSendString(close.c_str(), NULL, 0, NULL);
                vec.clear();
                goto repeat;
            }
        }
        // Load From File And Push It Into vec
        while (getline(finput,temp)) {
            vec.push_back(temp);
        }
        for (unsigned int x = 0; x != vec.size(); ++x) {
            cout << vec[x];
            switch (x) {
                case 2:
                    cout << path << name;
                    break;
                case 12:
                    cout << boolalpha << ifRepeat;
                    break;
                case 15:
                    for (long x = 0; x < (static_cast<double>(pos + 100) / length) * 38; ++x) {
                        cout << '=';
                    }
                    cout << pos << '/' << length;
                    break;
                default:
                    ;
            }
            if (x == line) {
                cout << " <<";
            }
            cout << '\n';
        }
        // Need Multithreading
        // if (refresh) {
        //     Sleep(100);
        //     goto refreshpoint;
        // }
        line = screenOperation();
        if (line == 3 && ifResume) {
            ifResume = false;
            ifPaused = true;
            ifChange = true;
            para = "pause music";
        }
        if (line == 4 && ifPaused) {
            ifPaused  = false;
            ifResume = true;
            ifChange = true;
            para = "resume music";
        }
        switch (line) {
            case 5:
                mciSendString(close.c_str(), NULL, 0, NULL);
                return;
            case 6:
                if (ifRepeat) {
                    ifRepeat = false;
                } else {
                    ifRepeat = true;
                }
                break;
            case 7:
                refresh = true;
                break;
            case 8:
            // Not Finished
                if (!ifRepeat && pos == length) {
                    listloop = true;
                    PlayerScreen(path,list[ival ++]);
                    if (ival == list.size()){
                        system("cls");
                        cout << "Reached The End Of The Music List!";
                    }
                    return;
                }
                break;
            default:
                ;
        }
        if (ifChange) {
            mciSendString(para.c_str(), NULL, 0, NULL);
            ifChange = false;
        }
    }
}

int screenOperation() {
    char c = getch();
    switch (c) {
        case 'p':
            return 3;
        case 'r':
            return 4;
        case 's':
            return 5;
        case 'o':
            return 6;
        case 'h':
            return 7;
        case 'a':
            return 8;
        default:
            ;
    }
    return 0;
}

vector<string> loadfile(string path) {
    string temp;
    vector<string> vec;
    ifstream finput(path);
    while (getline(finput,temp)) {
        vec.push_back(temp);
    }
    return vec;
}