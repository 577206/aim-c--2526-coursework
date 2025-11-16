#include "Song.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;

// ------------------------------------------------------
// 匿名命名空间：辅助函数
// ------------------------------------------------------
namespace {

string trim_copy(const string &s) {
    size_t start = 0, end = s.size();
    while (start < end && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    return s.substr(start, end - start);
}

string to_lower_copy(const string &s) {
    string t = s;
    std::transform(t.begin(), t.end(), t.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return t;
}

string join_tags(const vector<string> &tags) {
    std::ostringstream oss;
    for (size_t i = 0; i < tags.size(); ++i) {
        if (i > 0)
            oss << ", ";
        oss << tags[i];
    }
    return oss.str();
}

} // anonymous namespace

// ------------------------------------------------------
// 静态成员初始化
// ------------------------------------------------------
int Song::next_id_ = 1;

// ------------------------------------------------------
// 构造函数
// ------------------------------------------------------
Song::Song(const string &title, const string &artist, int duration_sec, int rating) {
    string t = trim_copy(title);
    string a = trim_copy(artist);

    bool ok = true;

    if (t.empty()) {
        std::cout << "[错误] 标题不能为空\n";
        ok = false;
    }
    if (a.empty()) {
        std::cout << "[错误] 艺人不能为空\n";
        ok = false;
    }
    if (duration_sec <= 0) {
        std::cout << "[错误] 时长必须为正整数（秒）\n";
        ok = false;
    }
    if (rating < 1 || rating > 5) {
        std::cout << "[错误] 评分必须在 1...5 之间\n";
        ok = false;
    }

    if (!ok) {
        valid_ = false;
        return;
    }

    // 合法
    id_ = next_id_++;
    title_ = t;
    artist_ = a;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

// ------------------------------------------------------
// Setters
// ------------------------------------------------------
bool Song::set_title(const string &t) {
    string x = trim_copy(t);
    if (x.empty()) {
        std::cout << "[提示] 标题不能为空，已忽略本次修改\n";
        return false;
    }
    title_ = x;
    return true;
}

bool Song::set_artist(const string &a) {
    string x = trim_copy(a);
    if (x.empty()) {
        std::cout << "[提示] 艺人不能为空，已忽略本次修改\n";
        return false;
    }
    artist_ = x;
    return true;
}

bool Song::set_duration(int sec) {
    if (sec <= 0) {
        std::cout << "[提示] 时长需为正整数，已忽略本次修改\n";
        return false;
    }
    duration_sec_ = sec;
    return true;
}

bool Song::set_rating(int r) {
    if (r < 1 || r > 5) {
        std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n";
        return false;
    }
    rating_ = r;
    return true;
}

// ------------------------------------------------------
// 标签管理
// ------------------------------------------------------
bool Song::add_tag(const string &tag) {
    string x = trim_copy(tag);
    if (x.empty()) {
        std::cout << "[提示] 空标签已忽略\n";
        return false;
    }

    string x_low = to_lower_copy(x);

    for (const auto &t : tags_) {
        if (to_lower_copy(t) == x_low) {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }

    tags_.push_back(x);
    return true;
}

bool Song::remove_tag(const string &tag) {
    string x_low = to_lower_copy(trim_copy(tag));

    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == x_low) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }

    std::cout << "[提示] 未找到该标签\n";
    return false;
}

// ------------------------------------------------------
// 关键词匹配
// ------------------------------------------------------
bool Song::matches_keyword(const string &kw) const {
    string k = trim_copy(kw);
    if (k.empty())
        return false;

    string low_k = to_lower_copy(k);

    if (to_lower_copy(title_).find(low_k) != string::npos)
        return true;
    if (to_lower_copy(artist_).find(low_k) != string::npos)
        return true;

    for (const auto &t : tags_) {
        if (to_lower_copy(t).find(low_k) != string::npos)
            return true;
    }

    return false;
}

// ------------------------------------------------------
// 输出格式
// ------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Song &s) {
    os << "[#" << s.id_ << "] "
       << s.artist_ << " - " << s.title_
       << " (" << s.duration_sec_ << "s) "
       << string(s.rating_, '*');

    if (!s.tags_.empty()) {
        os << " [tags: " << join_tags(s.tags_) << "]";
    }

    return os;
}

// ------------------------------------------------------
// 排序规则
// ------------------------------------------------------
bool operator<(const Song &a, const Song &b) {
    if (a.rating_ > b.rating_)
        return true;
    if (a.rating_ < b.rating_)
        return false;

    if (a.title_ < b.title_)
        return true;
    if (a.title_ > b.title_)
        return false;

    return a.id_ < b.id_;
}
