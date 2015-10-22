#include "boilerplate.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <ncurses.h>
#include <stdlib.h>

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

bool Boilerplate::run() {

  ifstream inFile("./boilerplate.cpp");

  vector<string> lines;
  
  int highlight_x=10;
  int highlight_y=12;

  string test = "";
  
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  for (string line; getline(inFile, line);)
    lines.push_back(line);

  inFile.close();

  int ch;
  int maxx,maxy;
  getmaxyx(stdscr,maxy,maxx);
  
  int scrolloffset = 0;

  bool dontquit = true;
  
  while (dontquit)
    {
      clear();

      move(0,0);
      attron(A_STANDOUT);
      printw("bootstraps %d // press F5 to continue", highlight_y-1+scrolloffset);
      attroff(A_STANDOUT);
  
      for(int y=0;y<maxy-1;y++)
	{
	  int ytoshow = y + scrolloffset;
	  
	  if (lines.size()>ytoshow && ytoshow >= 0) {
	    for(int x=0;x<lines[ytoshow].length();x++)
	      {
		move(y+1,x); // extra 1 is for header
		addch(lines[ytoshow].c_str()[x]);
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
      int file_y = highlight_y-1+scrolloffset;
      
      lines_it = lines.begin()+file_y;
      line = *lines_it;
      line_it = line.begin()+file_x;

      switch (ch) {
      case '\n':
	lines.insert(lines_it+1,"");
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
	
      case KEY_BACKSPACE:
	  if (line == "") {
	    lines.erase(lines_it);
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

      default:
	if (highlight_x < line.size()) {
	  line.insert(line_it, ch);
	  highlight_x++;
	  *lines_it = line;
	} else {
	  line.push_back(ch);
	  *lines_it = line;
	}
      }
	
      if (highlight_y>maxy) {
	highlight_y -= 1;
	if (file_y < lines.size()-1)
	  scrolloffset += 1;
      }
      if (highlight_y<1) {
	highlight_y += 1;
	scrolloffset -= 1;
      }
      if (highlight_x<0) highlight_x = 0;
    }
  
  endwin();

  // Here's the fun part

  ofstream oFile("boilerplate.cpp");
  for (string i : lines) {
    oFile << i << "\n";
  }
  oFile.close();
  
  int error = system("clear;g++ -std=c++11 -fPIC -shared boilerplate.cpp -o boilerplate.so -lncurses");

  if (error != 0) // good job
    {
      cout << ":(" << endl;
      getch();
    }
  
  return true;
}


