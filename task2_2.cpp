/*
c++17 required!
*/

#include<iostream>
#include<sstream>
#include<string>        // std::string
#include<vector>        // std::vector
#include<utility>       // std::pair
#include<tuple>         // std::tuple
#include<set>           // std::set

struct Printer {
    std::string str() const{
        return tmp_res.str();
    }

    template<typename T>
    Printer& format( const T& val ){
        tmp_res << val;
        return *this;
    }

    template<class InternalT1, class InternalT2>
    Printer& format( const std::pair<InternalT1, InternalT2>& val){
        tmp_res << "(";
        format(val.first);
        tmp_res << ", ";
        format(val.second);
        tmp_res << ")";
        return *this;
    }

    template<class InternalT>
    Printer& format( const std::vector<InternalT>& val){
        tmp_res << "[ ";
        if (val.empty()){
            tmp_res << "]";
            return *this;
        }

        for (const auto& v: val){
            format(v);
            tmp_res << ", ";
        }
        tmp_res << "\b\b ]";
        return *this;
    }

    template<class InternalT>
    Printer& format( const std::set<InternalT>& val){
        tmp_res << "{ ";
        if (val.empty()){
            tmp_res << "}";
            return *this;
        }

        for (const auto& v: val){
            format(v);
            tmp_res << ", ";
        }
        tmp_res << "\b\b }";
        return *this;
    }


    template<size_t I = 0, typename... Tp>
    Printer& format(std::tuple<Tp...>& t) {
        if constexpr(I == 0){
            tmp_res << "( ";
        }
        format(std::get<I>(t));
        tmp_res << ", ";
        if constexpr(I+1 != sizeof...(Tp))
            return format<I+1>(t);
        else {
            tmp_res << "\b\b )";
            return *this;
        }
    }

private:
    std::stringstream tmp_res;
};

int main(){
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
    std::string s1 = Printer().format(" vector: ").format(v).str();
    std::cout << s1 << std::endl;

    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s2 << std::endl;
    return 0;
}