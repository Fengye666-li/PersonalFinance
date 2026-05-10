#ifndef BILL_H
#define BILL_H

#include<string>
#include<vector>
#include<iomanip>

class Bill{
private:
    static int nextId;
    int id;
    std::string date;//格式
    std::string type;//收入还是支出
    std::string category;
    double amount;
    std::string note;

public:
    Bill();
    Bill(const std::string& d,const std::string& t,const std::string& cat,
         double amount,const std::string& nt = "");
    
    //读取的,不可以改
    int getId() const {return id;}
    //后面的 const 表示这个方法不会修改对象内部的任何数据。
    std::string getDate() const {return date;}
    std::string getType() const {return type;}
    std::string getCategory() const {return category;}
    double getAmount() const {return amount;}
    std::string getNote() const {return note;}

    //可修改的
    void setNote(const std::string& nt) {note = nt;}

    double getSignedAmount() const{
        return (type == "收入") ? amount : -amount;
    }

    void display() const;

    //文件读写
    std::string toFileString() const;
    static Bill fromFileString(const std::string& line);

    //重置ID
    static void resetNextId(int newId) { nextId = newId; }
    static int getNextId() { return nextId; }
};

#endif