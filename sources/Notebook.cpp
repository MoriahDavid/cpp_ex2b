#include "Notebook.hpp"
#include "Direction.hpp"
#include <string>
#include <iostream>

const size_t MAX_COL_VAL = 100;
const size_t MAX_ROW_VAL = 100;
const int MIN_CHAR_PRINTABLE_VAL = 32;
const int MAX_CHAR_PRINTABLE_VAL = 126;

using namespace std;

namespace ariel {
	
    Notebook::Notebook(){
    }

    void Notebook::write(int page, int row, int column, Direction d, string s){

        if(d != Direction::Horizontal && d != Direction::Vertical){
            throw invalid_argument("Direction incorrect");
        }
        //check if row collom page positive
        if(row < 0 || column < 0){
            throw invalid_argument("row and column must be positive");
        }
        //check if page exsist
        if(page < 0){
            throw invalid_argument("Page number must be positive");
        }
        //check if chars legall
        for(size_t i = 0; i < s.length(); i++){
            if(s[i] < MIN_CHAR_PRINTABLE_VAL || s[i] > MAX_CHAR_PRINTABLE_VAL || s[i] == '~'){
                throw invalid_argument("char must be ascii");
            }
        }

        if(umap.find(page) == umap.end()){ // page not exist
            if(d == Direction::Horizontal){
                umap[page] = vector<vector<char>> ((size_t) row+1, vector<char>(MAX_COL_VAL, '_'));
            }
            else{
                umap[page] = vector<vector<char>> ((size_t) row+1+s.length(), vector<char>(MAX_COL_VAL, '_'));
            }    
        }
        else{ // page exist
            if(umap[page].size() < (size_t) row+1 && d == Direction::Horizontal){
                umap[page].resize((size_t) row+1, vector<char>(MAX_COL_VAL, '_'));
            }
            else if(umap[page].size() < (size_t) row+1+s.length() && d == Direction::Vertical){
                umap[page].resize((size_t) row+1+s.length(), vector<char>(MAX_COL_VAL, '_'));
            }
        }

        //check if horizonal and row + chars > 100 ---> error
        if(d == Direction::Horizontal){
            if((size_t) column + s.length() > MAX_COL_VAL){
                throw invalid_argument("Too long for this row");
            }
            //check if ~ there ---> error
            for(size_t i = 0; i < s.length(); i++){
                if(umap[page][(size_t) row][(size_t) column+i] == '~'){
                    throw invalid_argument("Cant write on deleted area");
                }
                if(umap[page][(size_t) row][(size_t) column+i] != '_'){
                    throw invalid_argument("Cant write on written area");
                }
            }
        }
        else if(d == Direction::Vertical){
            if((size_t) column >= MAX_COL_VAL){
                throw invalid_argument("Too long for this row");
            }
            //check if ~ there ---> error
            for(size_t i = 0; i < s.length(); i++){
                if(umap[page][(size_t) row+i][(size_t) column] == '~'){
                    throw invalid_argument("Cant write on deleted area");
                }
                if(umap[page][(size_t) row+i][(size_t) column] != '_'){
                    throw invalid_argument("Cant write on written area");
                }
            }
        }
        //write
        if (d == Direction::Horizontal){
            for(size_t i = 0; i < s.length(); i++){
                umap[page][(size_t) row][(size_t) column+i] = s[i];
            }  
        }
        else if (d == Direction::Vertical){
            for(size_t i = 0; i < s.length(); i++){
                umap[page][(size_t) row+i][(size_t) column] = s[i];
            }  
        }
    }


    string Notebook::read(int page, int row, int column, Direction d, int n){

        if(row < 0 || column < 0){
            throw invalid_argument("row and column must be positive");
        }
        //check if page exsist
        if(page < 0){
            throw invalid_argument("Page number must be positive");
        }
        //check if number positive
        if(n < 0){
            throw invalid_argument("Page number must be positive");
        }
        if(d != Direction::Horizontal && d != Direction::Vertical){
            throw invalid_argument("Direction incorrect");
        }
        if(d == Direction::Horizontal && column + n > MAX_COL_VAL){
            throw invalid_argument("Too long for this row");
        }
        if(d == Direction::Vertical && column >= MAX_COL_VAL){
            throw invalid_argument("Too long for this row");
        }
        if(umap.find(page) == umap.end()){
            return string((size_t) n, '_');
        }
        
        string s;
        
        //read
        if (d == Direction::Horizontal){
            for(size_t i = 0; i < n; i++){
                s += umap[page][(size_t) row][(size_t) column+i];
            }  
        }
        else if (d == Direction::Vertical){
            for(size_t i = 0; i < n && (size_t)row+i < umap[page].size(); i++){
                s += umap[page][(size_t) row+i][(size_t) column];
            }  
            if (row + n >= umap[page].size()){ // Add '_' after
                s += string((size_t) (row + n) - umap[page].size(), '_');
            }
        }

        return s;
    }


    void Notebook::erase(int page, int row, int column, Direction d, int n){

        //check if row collom page positive
        if(row < 0 || column < 0){
            throw invalid_argument("row and column must be positive");
        }
        //check if page exsist
        if(page < 0){
            throw invalid_argument("Page number must be positive");
        }
        //check if number positive
        if(n < 0){
            throw invalid_argument("Page number must be positive");
        }
        if(d != Direction::Horizontal && d != Direction::Vertical){
            throw invalid_argument("Direction incorrect");
        }
        if(d == Direction::Horizontal && column + n >= MAX_COL_VAL){
            throw invalid_argument("Too long for this row");
        }
        if(d == Direction::Vertical && column >= MAX_COL_VAL){
            throw invalid_argument("Too long for this row");
        }

        if(umap.find(page) == umap.end()){ // page not exist
            if(d == Direction::Horizontal){
                umap[page] = vector<vector<char>> ((size_t) row+1, vector<char>(MAX_COL_VAL, '_'));
            }
            else{
                umap[page] = vector<vector<char>> ((size_t) (row+1+n), vector<char>(MAX_COL_VAL, '_'));
            }    
        }
        else{ // page exist
            if(umap[page].size() < (size_t) row+1 && d == Direction::Horizontal){
                umap[page].resize((size_t) row+1, vector<char>(MAX_COL_VAL, '_'));
            }
            else if(umap[page].size() < (size_t) row + (size_t) n + 1 && d == Direction::Vertical){
                umap[page].resize((size_t) row + (size_t) n + 1, vector<char>(MAX_COL_VAL, '_'));
            }
        }
        
        if (d == Direction::Vertical && column + n > umap[page].size()){
            umap[page].resize((size_t) row + (size_t) n + 1, vector<char>(MAX_COL_VAL, '_'));
        }

        if (d == Direction::Horizontal){
            for(size_t i = 0; i < n; i++){
                umap[page][(size_t) row][(size_t) column+i] = '~';
            }  
        }
        else if (d == Direction::Vertical){
            for(size_t i = 0; i < n; i++){
                umap[page][(size_t) row+i][(size_t) column] = '~';
            }  
        }
    }


    void Notebook::show(int page_num){

        string empty_line = string((size_t) MAX_COL_VAL, '_');

        //check if page exist
        if(page_num < 0){
            throw invalid_argument("Page number must be positive");
        }
        if(umap.find(page_num) == umap.end()){ // page not exist
            for(int i = 0; i < MAX_ROW_VAL; i++){
                cout<<empty_line<<endl; //print row in len 100
            }
            return;   
        }
        for(int i = 0; i < MAX_ROW_VAL && i < umap[page_num].size(); i++){
            for(int j = 0; j < MAX_COL_VAL; j++){
                cout<<umap[page_num][(size_t) i][(size_t) j];
            }
            cout<<endl;
        }
        for(int i = umap[page_num].size(); i < MAX_ROW_VAL; i++){
            cout<<empty_line<<endl;
        }
    }
};
