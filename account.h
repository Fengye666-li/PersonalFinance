#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Bill.h"
#include <vector>
#include <string>

class Account {
private:
    std::vector<Bill> bills;  // 存储所有账单
    double budget;             // 月度预算
    
    // 按日期排序
    void sortByDate();
    
public:
    Account();
    
    // 账单操作
    void addBill(const Bill& bill);
    bool removeBill(int id);
    bool editNote(int id, const std::string& newNote);
    Bill* findBill(int id);
    
    // 统计
    double getTotalIncome() const;
    double getTotalExpense() const;
    double getBalance() const;
    int getCount() const { return bills.size(); }
    
    // 查询
    std::vector<Bill> getBillsByMonth(int year, int month) const;
    std::vector<Bill> getBillsByType(const std::string& type) const;
    std::vector<Bill> getBillsByCategory(const std::string& category) const;
    std::vector<Bill> searchByKeyword(const std::string& keyword) const;
    
    // 月度报告
    struct MonthData {
        int year;
        int month;
        double income;
        double expense;
        double balance;
    };
    std::vector<MonthData> getMonthlySummary() const;
    
    // 分类统计
    struct CategoryStat {
        std::string category;
        double amount;
        int count;
    };
    std::vector<CategoryStat> getCategoryStats(const std::string& type) const;
    
    // 预算
    void setBudget(double b) { budget = b; }
    double getBudget() const { return budget; }
    double getCurrentMonthExpense() const;  // 当月支出
    double getRemainingBudget() const;      // 当月剩余预算
    
    // 显示
    void displayAll() const;
    void displayBills(const std::vector<Bill>& bills) const;
    
    // 文件
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif