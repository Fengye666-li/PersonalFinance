#include "Account.h"      // 账户类头文件
#include <fstream>        // 文件读写
#include <sstream>        // 字符串流处理
#include <algorithm>      // 算法（排序、查找等）
#include <iostream>       // 输入输出
#include <iomanip>        // 格式化输出（setw等）
#include <ctime>          // 时间操作（获取当前日期）

// ==================== 构造函数 ====================
// 默认构造函数：初始化预算为0
Account::Account() : budget(0) {}

// ==================== 排序功能 ====================
// 按日期排序账单（升序）
void Account::sortByDate() {
    std::sort(bills.begin(), bills.end(),
        [](const Bill& a, const Bill& b) {  // Lambda表达式作为比较函数
            return a.getDate() < b.getDate();  // 字符串日期可以直接比较
        });
}

// ==================== 账单管理 ====================
// 添加账单：添加到列表后自动按日期排序
void Account::addBill(const Bill& bill) {
    bills.push_back(bill);  // 添加到末尾
    sortByDate();           // 重新排序保持日期顺序
}

// 删除账单：根据ID删除
bool Account::removeBill(int id) {
    // 在bills中查找指定ID的账单
    auto it = std::find_if(bills.begin(), bills.end(),
        [id](const Bill& b) { return b.getId() == id; });
    
    if (it != bills.end()) {  // 找到了
        bills.erase(it);       // 删除该元素
        return true;           // 删除成功
    }
    return false;              // 未找到
}

// 修改备注：根据ID修改备注内容
bool Account::editNote(int id, const std::string& newNote) {
    Bill* bill = findBill(id);  // 查找账单
    if (bill) {
        bill->setNote(newNote);  // 修改备注
        return true;
    }
    return false;
}

// 查找账单：根据ID返回指针（用于内部操作）
Bill* Account::findBill(int id) {
    for (auto& b : bills) {
        if (b.getId() == id) {
            return &b;  // 返回地址
        }
    }
    return nullptr;  // 未找到返回空指针
}

// ==================== 统计查询 ====================
// 计算总收入
double Account::getTotalIncome() const {
    double total = 0;
    for (const auto& b : bills) {
        if (b.getType() == "收入") {
            total += b.getAmount();
        }
    }
    return total;
}

// 计算总支出
double Account::getTotalExpense() const {
    double total = 0;
    for (const auto& b : bills) {
        if (b.getType() == "支出") {
            total += b.getAmount();
        }
    }
    return total;
}

// 计算结余 = 收入 - 支出
double Account::getBalance() const {
    return getTotalIncome() - getTotalExpense();
}

// 获取指定年月的账单
std::vector<Bill> Account::getBillsByMonth(int year, int month) const {
    std::vector<Bill> result;
    // 构造前缀：如 "2026-05"（月份自动补零）
    std::string prefix = std::to_string(year) + "-" + 
                         (month < 10 ? "0" + std::to_string(month) : std::to_string(month));
    
    // 筛选日期以该前缀开头的账单
    for (const auto& b : bills) {
        if (b.getDate().substr(0, 7) == prefix) {  // 取日期字符串的前7个字符（YYYY-MM）
            result.push_back(b);
        }
    }
    return result;
}

// 按类型筛选账单（收入/支出）
std::vector<Bill> Account::getBillsByType(const std::string& type) const {
    std::vector<Bill> result;
    for (const auto& b : bills) {
        if (b.getType() == type) {
            result.push_back(b);
        }
    }
    return result;
}

// 按类别筛选账单（餐饮、交通等）
std::vector<Bill> Account::getBillsByCategory(const std::string& category) const {
    std::vector<Bill> result;
    for (const auto& b : bills) {
        if (b.getCategory() == category) {
            result.push_back(b);
        }
    }
    return result;
}

// 关键词搜索：在备注中查找关键词
std::vector<Bill> Account::searchByKeyword(const std::string& keyword) const {
    std::vector<Bill> result;
    for (const auto& b : bills) {
        // find返回 npos 表示未找到
        if (b.getNote().find(keyword) != std::string::npos) {
            result.push_back(b);
        }
    }
    return result;
}

// ==================== 报表统计 ====================

// 月度汇总数据结构
// struct MonthData { int year; int month; double income; double expense; double balance; };

// 获取每月收支汇总
std::vector<Account::MonthData> Account::getMonthlySummary() const {
    std::vector<MonthData> summary;
    
    // 遍历所有账单，汇总每月收支
    for (const auto& b : bills) {
        // 从日期字符串提取年月：如 "2026-05-10" -> 2026, 5
        std::string date = b.getDate();
        int year = std::stoi(date.substr(0, 4));   // 前4位：年份
        int month = std::stoi(date.substr(5, 2));  // 第6-7位：月份
        
        bool found = false;
        // 查找是否已有该月份的汇总
        for (auto& m : summary) {
            if (m.year == year && m.month == month) {
                if (b.getType() == "收入") {
                    m.income += b.getAmount();    // 累加收入
                } else {
                    m.expense += b.getAmount();   // 累加支出
                }
                m.balance = m.income - m.expense;  // 更新结余
                found = true;
                break;
            }
        }
        
        // 如果是新月份，创建新的汇总项
        if (!found) {
            MonthData m{year, month, 0, 0, 0};
            if (b.getType() == "收入") {
                m.income = b.getAmount();
            } else {
                m.expense = b.getAmount();
            }
            m.balance = m.income - m.expense;
            summary.push_back(m);
        }
    }
    
    // 按年月排序（先按年，再按月）
    std::sort(summary.begin(), summary.end(),
        [](const MonthData& a, const MonthData& b) {
            if (a.year != b.year) return a.year < b.year;
            return a.month < b.month;
        });
    
    return summary;
}

// 分类统计数据（按收入类型或支出类型统计）
// struct CategoryStat { std::string category; double amount; int count; };
std::vector<Account::CategoryStat> Account::getCategoryStats(const std::string& type) const {
    std::vector<CategoryStat> stats;
    
    // 只统计指定类型的账单（收入或支出）
    for (const auto& b : bills) {
        if (b.getType() != type) continue;
        
        bool found = false;
        // 查找是否已有该分类
        for (auto& s : stats) {
            if (s.category == b.getCategory()) {
                s.amount += b.getAmount();  // 累加金额
                s.count++;                   // 次数+1
                found = true;
                break;
            }
        }
        
        // 新分类：添加统计项
        if (!found) {
            stats.push_back({b.getCategory(), b.getAmount(), 1});
        }
    }
    
    // 按金额降序排序（从高到低）
    std::sort(stats.begin(), stats.end(),
        [](const CategoryStat& a, const CategoryStat& b) {
            return a.amount > b.amount;
        });
    
    return stats;
}

// 获取当前月份的支出总额（用于预算检查）
double Account::getCurrentMonthExpense() const {
    // 获取当前系统时间
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int year = now->tm_year + 1900;  // tm_year从1900年开始
    int month = now->tm_mon + 1;      // tm_mon范围0-11
    
    double expense = 0;
    for (const auto& b : bills) {
        std::string date = b.getDate();
        int y = std::stoi(date.substr(0, 4));
        int m = std::stoi(date.substr(5, 2));
        
        // 筛选当前月份且为支出的账单
        if (y == year && m == month && b.getType() == "支出") {
            expense += b.getAmount();
        }
    }
    return expense;
}

// 获取剩余预算 = 总预算 - 本月已支出
double Account::getRemainingBudget() const {
    return budget - getCurrentMonthExpense();
}

// ==================== 显示功能 ====================

// 显示所有账单
void Account::displayAll() const {
    if (bills.empty()) {
        std::cout << "\n暂无账单记录。\n";
        return;
    }
    
    // 打印表头（使用 setw 格式化对齐）
    std::cout << "\n" << std::string(65, '=') << std::endl;
    std::cout << std::left 
              << std::setw(5) << "ID"
              << std::setw(12) << "日期"
              << std::setw(8) << "类型"
              << std::setw(10) << "类别"
              << std::setw(10) << "金额"
              << "备注" << std::endl;
    std::cout << std::string(65, '-') << std::endl;
    
    // 逐条显示账单
    for (const auto& b : bills) {
        b.display();  // 调用 Bill 类的显示方法
    }
    
    // 显示统计信息
    std::cout << std::string(65, '-') << std::endl;
    std::cout << "总收入: ¥" << std::fixed << std::setprecision(2) << getTotalIncome() << std::endl;
    std::cout << "总支出: ¥" << getTotalExpense() << std::endl;
    std::cout << "结余:   ¥" << getBalance() << std::endl;
}

// 显示指定的账单列表（用于查询结果）
void Account::displayBills(const std::vector<Bill>& billsToShow) const {
    if (billsToShow.empty()) {
        std::cout << "\n没有找到符合条件的账单。\n";
        return;
    }
    
    // 打印表头
    std::cout << std::left 
              << std::setw(5) << "ID"
              << std::setw(12) << "日期"
              << std::setw(8) << "类型"
              << std::setw(10) << "类别"
              << std::setw(10) << "金额"
              << "备注" << std::endl;
    std::cout << std::string(65, '-') << std::endl;
    
    // 逐条显示
    for (const auto& b : billsToShow) {
        b.display();
    }
}

// ==================== 文件持久化 ====================

// 保存数据到文件
bool Account::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);  // 打开输出文件流
    if (!file) {                    // 检查是否打开成功
        std::cout << "无法保存文件！\n";
        return false;
    }
    
    // 保存预算（以 # 开头作为元数据标记）
    file << "#BUDGET:" << budget << std::endl;
    // 保存下一个可用ID（确保下次启动时ID连续）
    file << "#NEXT_ID:" << Bill::getNextId() << std::endl;
    
    // 保存每条账单（调用 toFileString 转换为字符串）
    for (const auto& b : bills) {
        file << b.toFileString() << std::endl;
    }
    
    file.close();
    return true;
}

// 从文件加载数据
bool Account::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);  // 打开输入文件流
    if (!file) {
        return false;  // 文件不存在不算错误（首次运行会创建）
    }
    
    bills.clear();      // 清空现有账单
    std::string line;
    
    // 逐行读取文件
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // 跳过空行
        
        // 处理元数据行（以 # 开头）
        if (line[0] == '#') {
            if (line.substr(0, 8) == "#BUDGET:") {
                // 解析预算：跳过 "#BUDGET:" 这8个字符
                budget = std::stod(line.substr(8));
            } else if (line.substr(0, 9) == "#NEXT_ID:") {
                // 解析下一个ID：跳过 "#NEXT_ID:" 这9个字符
                Bill::resetNextId(std::stoi(line.substr(9)));
            }
            continue;  // 元数据处理完，继续读下一行
        }
        
        // 普通行：账单数据，调用 fromFileString 解析
        Bill bill = Bill::fromFileString(line);
        bills.push_back(bill);
    }
    
    file.close();
    sortByDate();  // 按日期排序
    return true;
}