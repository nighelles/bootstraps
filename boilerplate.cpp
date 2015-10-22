#include "boilerplate.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <ncurses.h>
#include <stdlib.h>

const int header_size = 2;

using namespace std;
 
//This is a test comment 

extern "C" Boilerplate* create_Boilerplate()

{
  return new Boilerplate;
}

extern "C" void delete_Boilerplate( Boilerplate* bp )
{
  delete bp;
}

Boilerplate::Boilerplate() {
  ;;
}

void save(string filename, const vector<string> &lines) {
    ofstream oFile(filename);
    for (string i : lines) {
        oFile << i << "\n";
    }
    oFile.close();
  
}

bool Boilerplate::run() {

  vector<vector<string>> lines;

  vector<string> files{"./boilerplate.cpp","./boilerplate.h"};
  int file_num = 0;
  
  int highlight_x=10;
  int highlight_y=12;

  string test = "";
  
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  // load files
  for (string filename : files) {
    vector<string> tempLines;
    ifstream inFile(filename);

    for (string line; getline(inFile, line);)
      tempLines.push_back(line);
    
    inFile.close();
    lines.push_back(tempLines);
  }
  
  int ch;
  int maxx,maxy;
  getmaxyx(stdscr,maxy,maxx);
  
  int scrolloffset = 0;

  bool dontquit = true;
  bool toreturn = true;
  
  while (dontquit)
    {
      clear();

      move(0,0);
      attron(A_STANDOUT);
      printw("bootstraps %d // Compile, F6 Switch Files, F7 Quit", highlight_y-header_size+scrolloffset);
      attroff(A_STANDOUT);

      // Do the file selection thing
      move(1,0);
      for (int i=0;i<files.size();i++)
	{
	  if (i==file_num) {
	    attron(A_STANDOUT);
	  } 
	  printw(files[i].c_str());
	  attroff(A_STANDOUT);
	}
      
      for(int y=0;y<maxy-header_size;y++)
	{
	  int ytoshow = y + scrolloffset;
	  
	  if (lines[file_num].size()>ytoshow && ytoshow >= 0) {
	    for(int x=0;x<lines[file_num][ytoshow].length();x++)
	      {
		move(y+header_size,x); // extra 1 is for header
		addch(lines[file_num][ytoshow].c_str()[x]);
	      }
	    }
	}
      move(highlight_y,highlight_x);
      refresh();
      ch = getch();

      vector<string>::iterator lines_it;
      string line;
      string::iterator line_it;

      int file_x = highlight_x;
      int file_y = highlight_y-header_size+scrolloffset;
      
      lines_it = lines[file_num].begin()+file_y;
      line = *lines_it;
      line_it = line.begin()+file_x;

      switch (ch) {
      case '\n':
	lines[file_num].insert(lines_it+1,"");
	highlight_y++;
	break;
      case KEY_UP:
	highlight_y--;break;
      case KEY_DOWN:
	highlight_y++;break;
      case KEY_LEFT:
	highlight_x--;break;
      case KEY_RIGHT:
	highlight_x++;break;
      case KEY_F(6):
	file_num+=1;
	if (file_num >= files.size()) file_num = 0;
	break;
      case KEY_BACKSPACE:
	  if (line == "") {
	    lines[file_num].erase(lines_it);
	  } else {
	    if (highlight_x < line.size()) {
	      
	      line.erase(line_it-1);
	      highlight_x--;
	      *lines_it = line;
	    } else {
	      line.erase(line.end()-1);
	      *lines_it = line;
	    }
	    
	  }
        break;

      case KEY_F(5):
	dontquit=false;
	break;
      case KEY_F(7):
	dontquit=false;
	toreturn=false;
	break;

      default:
	if (highlight_x <= line.size()) {
	  line.insert(line_it+1, ch);
	  highlight_x++;
	  *lines_it = line;
	} else {
	  line.push_back(ch);
	  *lines_it = line;
	  highlight_x = line.size()+1;
	}
      }
      
      if (highlight_y>maxy) {
	highlight_y -= 1;
	if (file_y < lines.size()-1)
	  scrolloffset += 1;
      }
      if (highlight_y<header_size) {
	highlight_y += 1;
	scrolloffset -= 1;
      }
      if (highlight_x<0) highlight_x = 0;
    }
  
  endwin();

  // Here's the fun part

  for (int i=0;i<files.size();i++) {
    save(files[i],lines[i]); //save all files
  }
  int error = system("clear;g++ -std=c++11 -fPIC -shared boilerplate.cpp -o boilerplate.so -lncurses");

  if (error != 0) // good job
    {
      cout << ":(" << endl;
      getch();
    }
  
  return toreturn;
}


