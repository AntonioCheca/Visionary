#ifndef _CRITICS_H_
#define _CRITICS_H_

#include <fstream>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

bool compile(std::string name, std::vector<std::string> other_files) {
    bool compiled;
    name = "g++ -Wall -o " + name + " " + name + ".cpp";
    for(std::vector<std::string>::const_iterator it = other_files.begin(); it != other_files.end(); ++it)
      name += " " + (*it);
    name += " -std=c++11";

    FILE *compiler = popen(name.c_str() , "w");
    compiled = std::fgetc(compiler) == EOF;
    pclose(compiler);
    return compiled;
}

bool execute(std::string name, std::string out, std::string& out_string){
  std::ofstream os(out.c_str());
  if(!os)
    return 0;

  int fd[2];
  pipe(fd); // Buffer to read the output of program 'name'
  pid_t pd = fork();
  char char_seq[1024];
  int st, r;
  if(pd == 0){
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    execl(name.c_str(), name.c_str(), NULL);
  }
  else{
    close(fd[1]);
    wait(&st);
    while(r = read(fd[0], char_seq, 1024) ){
      for(int i = 0; i < r; i++){
        os << char_seq[i];
        out_string.push_back(char_seq[i]);
      }
    }
  }

  return 1;

}

/**@brief Counts the number of lines of a file, given as an ifstream
  */
unsigned int numberOfLines(std::string name){
  std::ifstream in(name.c_str());
  if(!in)
    return 0;
  char* c = new char[1024];
  int total = 0;
  in.getline(c, 1024);
  while(in){
    total++;
    in.getline(c,1024);
  }
  in.close();
  delete[] c;
  return total;
}

/**@brief Search for the number of identical characters on the same position between the two strings
  */
unsigned int brute_score(std::string in, std::string out){
  int min_both = std::min((int) in.size(), (int) out.size());
  int total = 0;
  for(int i = 0; i < min_both; i++)
    if(in[i] == out[i])
      total++;
  return total;
}

/**@brief Search for the number of characters appearing on both strings
  */
unsigned int distributed_score(std::string in, std::string out){
  int min_both = std::min((int) in.size(), (int) out.size());
  int total = 0;
  int ch[256][2];
  for(int i = 0; i < 256; i++){
    ch[i][0] = 0;
    ch[i][1] = 0;
  }
  for(int i = 0; i < min_both; i++){
    ch[in[i]][0]++;
    ch[out[i]][1]++;
  }
  for(int i = 0; i < 256; i++)
    total += std::min(ch[i][0], ch[i][1]);
  return total;
}

/**@brief Scores a line (out) from its similarities with another line (in)
  *@note The order in this function is irrelevant
  */
unsigned int score_line(std::string in, std::string out){
  if(in == out)
    return 100;
  else{
    int max_both = std::max((int) in.size(), (int) out.size());
    if(max_both == 0)
      max_both = 1;
    return (brute_score(in, out) + distributed_score(in, out))*50.0/max_both;
  }
}

/** @brief Scores an output file (out) from its similarities with an input file(in)
    @note The max score is
  */
double score(std::string name_in, std::string name_out){
  int total = 0;
  std::ifstream in(name_in.c_str());
  std::ifstream out(name_out.c_str());
  if(!in || !out)
    return 0;

  int n_in = numberOfLines(name_in);
  int n_out = numberOfLines(name_out);
  int min_both = std::min(n_in, n_out);
  int max_both = std::max(n_in, n_out);
  std::string line_in, line_out;

  for(int i = 0; i < min_both; i++){
    getline(in, line_in);
    getline(out, line_out);
    total += score_line(line_in, line_out);
  }
  in.close();
  out.close();
  if(max_both == 0)
    max_both = 1;
  return total*1.0/max_both;

}

std::vector<double> parallel_score(std::string name_in, std::string name_out, int num_prog){
  int total;
  std::vector<double> score;
  std::ifstream in(name_in.c_str());
  std::ifstream out(name_out.c_str());
  if(!in || !out)
    return std::vector<double>();

  int n_in;
  int n_out = numberOfLines(name_out);
  std::vector<std::string> lines_out;
  std::string s;
  for(int i = 0; i < n_out; i++){
    getline(out, s);
    lines_out.push_back(s);
  }


  for(int j = 0; j < num_prog; j++){
    total = 0;
    in >> n_in;
    in.ignore();
    int min_both = std::min(n_in, n_out);
    int max_both = std::max(n_in, n_out);
    std::string line_in;

    for(int i = 0; i < min_both; i++){
      getline(in, line_in);
      total += score_line(line_in, lines_out[i]);
    }
    for(int i = min_both; i < n_in; i++)
      in.ignore(1024, '\n');

    if(max_both == 0)
      max_both = 1;
    score.push_back(total*1.0/max_both);
  }
  in.close();
  out.close();

  return score;
}


#endif
