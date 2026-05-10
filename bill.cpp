#include "bill.h"
#include <sstream>//内存中的字符串格式化、解析和转换
#include <vector>
#include <iostream>

int Bill::nextId = 1;

Bill::Bill() :id(nextId++),amount(0){}

Bill::Bill(const std::string& d, const std::string& t,
           const std::string& cat, double amt, const std::string& nt)
    : id(nextId++), date(d), type(t), category(cat), amount(amt), note(nt) {}

void Bill::display() const {
    std::cout<<std::left
             <<std::setw(5) <<id
             <<std::setw(12) <<date
             << std::setw(8) << type
             << std::setw(10) << category
             << std::setw(10) << std::fixed << std::setprecision(2) << amount
             << note
             << std::endl;
}

std::string Bill::toFileString() const {
    std::stringstream ss;
    ss << id << "|" << date << "|" << type << "|" 
       << category << "|" << amount << "|" << note;
    return ss.str();
}

Bill Bill::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> parts;
    
    while (std::getline(ss, token, '|')) {
        parts.push_back(token);
    }
    
    Bill bill;  // 创建临时对象
    
    if (parts.size() >= 6) {
        // 跳过 parts[0]（旧ID），不使用
        bill.date = parts[1];
        bill.type = parts[2];
        bill.category = parts[3];
        bill.amount = std::stod(parts[4]);
        bill.note = parts[5];
        
        // ID 不在这里设置，由 Bill 构造函数自动分配
        // 或者后面统一调用 resetNextId
    }
    
    return bill;
}