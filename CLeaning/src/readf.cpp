//
// Created by HFD on 12/11/2018.
//

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

int count_lines(const std::string& filename){
    std::ifstream in(filename);
    return std::count(
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>(),
            '\n');

}

std::vector<int>
count_lines_int_files2(const std::vector<std::string>& files)
{
    std::vector<int> results;
    char c = 0;

    for (const auto& file : files) {
        results.push_back(count_lines(file));
    }
    return results;
}
std::vector<int>
count_lines_int_files(const std::vector<std::string>& files)
{
    std::vector<int> results;
    char c = 0;

    for (const auto& file : files) {
        int line_count = 0;
        std::ifstream in(file);

        while (in.get(c)){
            if (c=='\n'){
                line_count++;
            }
        }
        results.push_back(line_count);
    }
    return results;
}

std::vector<int>
count_lines_int_files3(const std::vector<std::string>& files)
{
    std::vector<int> results(files.size());
    std::transform(files.cbegin(), files.cend(),
    results.cbegin(), count_lines);
    return results;
}
//std::vector<int>
//count_lines_int_files4(const std::vector<std::string>& files)
//{
//    return files | transform()
//}
double average_score(const std::vector<int>& scores)
{
    int sum = 0;
    for (int score : scores) {
        sum += score;
    }
    return sum/(double)scores.size();
}

double average_score1(const std::vector<int>& scores)
{
    return std::accumulate(scores.cbegin(), scores.cend(),0)/(double)scores.size();
}

double average_score2(const std::vector<int>& scores)
{
    return std::accumulate(scores.cbegin(), scores.cend(),
                           1,
                           std::multiplies<int>());
}

int main()
{
    std::vector<std::string> filepath;
    filepath.push_back("C:\\Work\\cPlus\\rest_file\\2.cpp");
//    char s[] = "C:\\Work\\cPlus\\rest_file\\2.cpp";
    std::vector<int> filelines(count_lines_int_files(filepath));
//    filelines.push_back(count_lines_int_files(filepath));

    for (std::vector<int>::iterator it = filelines.begin(); it < filelines.end(); ++it) {
        std::cout<<*it<<std::endl;
    }
//    std::cout << filelines << std::endl;
    return 0;
}


