#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>


namespace prog_opts {

void parse(int argc, char** argv);

std::string input_file("");
std::string output_file("");

bool serialize = false;
bool deserialize = false;

};

void prog_opts::parse(int argc, char** argv){
    if (argc > 32){
        throw std::runtime_error("To many arguments!"); // 
    }
    const std::vector<std::string> args(argv + 1, argv + argc);
    bool _input = false;
    bool _output = false;

    for (const auto& arg : args) {
        if (arg == "-s"){
            serialize = true;
        }
        else if (arg == "-d"){
            deserialize = true;
        }
        else if (arg == "-i"){
            _input = true;
        }
        else if (arg == "-o"){
            _output = true;
        }

        /*filename*/
        else if (_input && input_file == ""){
            input_file = arg;
        }

        else if (_output && output_file == ""){
            output_file = arg;
        }
    }
    if (serialize == deserialize)
        throw std::runtime_error("Please specify workflow: either -s or -d");
    if (input_file == "" || output_file == "")
        throw std::runtime_error("Please specify both input and output files");
}


class GraphBin{
public:
    void serialize(const std::string& input_file, const std::string& output_file){
        graph_map.clear();
        
        from_txt_file(input_file.c_str());
        to_bin_file(output_file.c_str());
    }
    
    void deserialize(const std::string& input_file, const std::string& output_file){
        graph_map.clear();

        from_bin_file(input_file.c_str());
        to_txt_file(output_file.c_str());
    }

    void print_map() const{
        /*For testing*/
        for(const auto& elem: graph_map){
            std::cout << elem.first << std::endl;
            for(const auto& n_w: elem.second){
                std::cout << n_w.first << '\t' << n_w.second << std::endl;
            }
        }
    }
private:
    using graph_serializitation_map =
    std::unordered_map<unsigned int, std::vector<std::pair<unsigned int, unsigned short>>>;
    graph_serializitation_map graph_map;

    std::ifstream open_file_safe_read(const char* filename) const{
        std::ifstream file_data;
        file_data.open(filename);
        if (!file_data.is_open()){
            throw std::runtime_error("Failed to open input file");
        }
        return file_data;
    }

    std::ofstream open_file_safe_write(const char* filename) const{
        std::ofstream file_data;
        file_data.open(filename);
        if (!file_data.is_open()){
            throw std::runtime_error("Failed to open/create output file");
        }
        return file_data;
    }

    void from_txt_file(const char* filename){
        std::ifstream file_data = open_file_safe_read(filename);
        std::string tmp_line;
        unsigned int node1, node2;
        unsigned short weight;
        while (std::getline(file_data, tmp_line)){
            std::stringstream line(tmp_line);
            line >> node1;
            line >> node2;
            line >> weight;
            if (graph_map.count(node2) == 0){
                graph_map[node1].emplace_back(std::pair<unsigned int, unsigned short>(node2, weight));
            } else {
                graph_map[node2].emplace_back(std::pair<unsigned int, unsigned short>(node1, weight));
            }
        }
        file_data.close();
        
    }

    void to_bin_file(const char* filename){
        std::ofstream out(filename, std::ios_base::binary);
        unsigned int node;
        unsigned short weight;
        unsigned int size = graph_map.size();
        out.write(reinterpret_cast<char*>(&size), sizeof(size));  // total size
        for (auto& elem: graph_map){
            node = elem.first;
            size = elem.second.size();
            out.write(reinterpret_cast<char*>(&node), sizeof(node));
            out.write(reinterpret_cast<char*>(&size), sizeof(size));
            for (auto& adjacent: elem.second){
                node = adjacent.first;
                weight = adjacent.second;
                out.write(reinterpret_cast<char*>(&node), sizeof(node));
                out.write(reinterpret_cast<char*>(&weight), sizeof(weight));
            } 
        }
        out.close();
    }

    void from_bin_file(const char* filename){
        std::ifstream in(filename, std::ios_base::binary);
        unsigned int map_size=0, len=0, node1=0, node2=0;
        unsigned short weight=0;
        in.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));
        graph_map.reserve(map_size);
        for (unsigned i = 0; i < map_size; i++){
            in.read(reinterpret_cast<char*>(&node1), sizeof(node1));
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            graph_map[node1].reserve(len);
            for(unsigned j = 0; j < len; j++){
                in.read(reinterpret_cast<char*>(&node2), sizeof(node2));
                in.read(reinterpret_cast<char*>(&weight), sizeof(weight));
                graph_map[node1].emplace_back(std::pair<unsigned int, unsigned short>(node2, weight));
            }
        }
        in.close();
        return;
    }

    void to_txt_file(const char* filename){
        std::ofstream file_data = open_file_safe_write(filename);
        for(const auto& elem: graph_map){
            for(const auto& n_w: elem.second){
                file_data << elem.first << '\t' << n_w.first << '\t' << n_w.second << std::endl;
            }
        }
        file_data.close();
    }
};



int main(int argc, char** argv){
    try{
        prog_opts::parse(argc, argv);
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return 0;
    }

    GraphBin serializer;
    if (prog_opts::serialize){
        serializer.serialize(prog_opts::input_file, prog_opts::output_file);
    } else if (prog_opts::deserialize){
        serializer.deserialize(prog_opts::input_file, prog_opts::output_file);
    }
}