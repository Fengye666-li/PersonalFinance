#include "Account.h"
#include <iostream>
#include <limits>
#include <ctime>

using namespace std;

// 获取当前日期字符串
string getToday() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buf[11];
    sprintf(buf, "%04d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    return string(buf);
}

// 获取用户输入的整数
int getInt(const string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail() || val < minVal || val > maxVal) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "请输入 " << minVal << "~" << maxVal << " 之间的数字！\n";
        } else {
            cin.ignore(10000, '\n');
            return val;
        }
    }
}

// 获取用户输入的金额
double getAmount(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail() || val <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "请输入大于0的金额！\n";
        } else {
            cin.ignore(10000, '\n');
            return val;
        }
    }
}

// 获取日期（回车使用今天）
string getDate() {
    string date;
    cout << "日期(YYYY-MM-DD，直接回车使用今天): ";
    getline(cin, date);
    if (date.empty()) {
        return getToday();
    }
    return date;
}

// 显示菜单
void showMenu() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║       💰 个人记账系统              ║\n";
    cout << "╠════════════════════════════════════╣\n";
    cout << "║  1. 添加收入                      ║\n";
    cout << "║  2. 添加支出                      ║\n";
    cout << "║  3. 查看所有账单                  ║\n";
    cout << "║  4. 查看月度统计                  ║\n";
    cout << "║  5. 查看分类统计                  ║\n";
    cout << "║  6. 删除账单                      ║\n";
    cout << "║  7. 搜索备注                      ║\n";
    cout << "║  8. 预算设置                      ║\n";
    cout << "║  9. 保存并退出                    ║\n";
    cout << "╚════════════════════════════════════╝\n";
}

// 添加收入
void addIncome(Account& account) {
    cout << "\n--- 添加收入 ---\n";
    string date = getDate();
    double amount = getAmount("金额: ");
    
    cout << "类别(1.工资 2.红包 3.其他): ";
    int catChoice = getInt("", 1, 3);
    string category;
    switch(catChoice) {
        case 1: category = "工资"; break;
        case 2: category = "红包"; break;
        default: category = "其他";
    }
    
    cout << "备注(可选): ";
    string note;
    getline(cin, note);
    
    account.addBill(Bill(date, "收入", category, amount, note));
    cout << "✓ 收入添加成功！\n";
}

// 添加支出
void addExpense(Account& account) {
    cout << "\n--- 添加支出 ---\n";
    string date = getDate();
    double amount = getAmount("金额: ");
    
    cout << "类别(1.餐饮 2.购物 3.交通 4.娱乐 5.其他): ";
    int catChoice = getInt("", 1, 5);
    string category;
    switch(catChoice) {
        case 1: category = "餐饮"; break;
        case 2: category = "购物"; break;
        case 3: category = "交通"; break;
        case 4: category = "娱乐"; break;
        default: category = "其他";
    }
    
    cout << "备注(可选): ";
    string note;
    getline(cin, note);
    
    account.addBill(Bill(date, "支出", category, amount, note));
    cout << "✓ 支出添加成功！\n";
}

// 查看月度统计
void showMonthlyStats(Account& account) {
    auto summary = account.getMonthlySummary();
    
    if (summary.empty()) {
        cout << "\n暂无数据。\n";
        return;
    }
    
    cout << "\n" << string(55, '=') << endl;
    cout << left 
         << setw(10) << "年月"
         << setw(12) << "收入"
         << setw(12) << "支出"
         << setw(12) << "结余" << endl;
    cout << string(55, '-') << endl;
    
    for (const auto& m : summary) {
        string ym = to_string(m.year) + "-" + (m.month < 10 ? "0" + to_string(m.month) : to_string(m.month));
        cout << left 
             << setw(10) << ym
             << setw(12) << fixed << setprecision(2) << m.income
             << setw(12) << m.expense
             << setw(12) << m.balance;
        
        if (m.balance >= 0) {
            cout << " 😊";
        } else {
            cout << " 😢";
        }
        cout << endl;
    }
    
    cout << string(55, '=') << endl;
}

// 查看分类统计
void showCategoryStats(Account& account) {
    cout << "\n--- 支出分类统计 ---\n";
    auto stats = account.getCategoryStats("支出");
    
    if (stats.empty()) {
        cout << "暂无支出记录。\n";
        return;
    }
    
    double total = account.getTotalExpense();
    cout << left 
         << setw(12) << "类别"
         << setw(12) << "金额"
         << setw(10) << "占比"
         << "次数" << endl;
    cout << string(40, '-') << endl;
    
    for (const auto& s : stats) {
        double percent = total > 0 ? (s.amount / total * 100) : 0;
        cout << left 
             << setw(12) << s.category
             << setw(12) << fixed << setprecision(2) << s.amount
             << setw(9) << setprecision(1) << percent << "%"
             << s.count << endl;
    }
    cout << string(40, '-') << endl;
    cout << "总支出: ¥" << total << endl;
}

// 删除账单
void deleteBill(Account& account) {
    account.displayAll();
    if (account.getCount() == 0) return;
    
    int id = getInt("\n要删除的账单ID: ", 1, 9999);
    if (account.removeBill(id)) {
        cout << "✓ 删除成功！\n";
    } else {
        cout << "未找到该ID的账单。\n";
    }
}

// 搜索备注
void searchNotes(Account& account) {
    cout << "请输入关键词: ";
    string keyword;
    getline(cin, keyword);
    
    auto results = account.searchByKeyword(keyword);
    account.displayBills(results);
}

// 预算设置
void budgetSettings(Account& account) {
    cout << "\n当前月度预算: ¥" << fixed << setprecision(2) << account.getBudget() << endl;
    cout << "本月已支出: ¥" << account.getCurrentMonthExpense() << endl;
    
    if (account.getBudget() > 0) {
        double remaining = account.getRemainingBudget();
        if (remaining >= 0) {
            cout << "本月剩余: ¥" << remaining << endl;
        } else {
            cout << "⚠️ 已超支 ¥" << -remaining << endl;
        }
    }
    
    cout << "\n是否设置新预算？(y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        double newBudget = getAmount("请输入月度预算: ");
        account.setBudget(newBudget);
        cout << "✓ 预算已更新\n";
    }
    cin.ignore();
}

int main() {
    Account account;
    const string dataFile = "bills.txt";  // 直接保存在当前目录
    
    // 加载已有数据
    if (account.loadFromFile(dataFile)) {
        cout << "✓ 加载成功，共 " << account.getCount() << " 条记录\n";
    } else {
        cout << "首次使用，将创建新账本\n";
    }
    
    int choice;
    do {
        showMenu();
        choice = getInt("请选择: ", 1, 9);
        
        switch (choice) {
            case 1: addIncome(account); break;
            case 2: addExpense(account); break;
            case 3: account.displayAll(); break;
            case 4: showMonthlyStats(account); break;
            case 5: showCategoryStats(account); break;
            case 6: deleteBill(account); break;
            case 7: searchNotes(account); break;
            case 8: budgetSettings(account); break;
            case 9:
                if (account.saveToFile(dataFile)) {
                    cout << "✓ 数据已保存，再见！\n";
                } else {
                    cout << "保存失败！\n";
                }
                break;
        }
        
        if (choice != 9) {
            cout << "\n按回车键继续...";
            cin.get();
        }
        
    } while (choice != 9);
    
    return 0;
}