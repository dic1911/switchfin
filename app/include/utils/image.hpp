#pragma once

#include <borealis.hpp>
#include "api/http.hpp"
#include "config.hpp"

class Image {
    using Ref = std::shared_ptr<Image>;

public:
    Image();
    Image(const Image&) = delete;

    virtual ~Image();

    template <typename... Args>
    static void load(brls::Image* view, const fmt::string_view fmt, Args&&... args) {
        std::string path = fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...);
        return with(view, AppConfig::instance().getUrl() + path);
    }

    /// @brief 设置要加载内容的图片组件。此函数需要工作在主线程。
    static void with(brls::Image* view, const std::string& url);

    /// @brief 取消请求，并清空图片。此函数需要工作在主线程。
    static void cancel(brls::Image* view);

private:
    void doRequest();

    static void clear(brls::Image* view);

private:
    std::string url;
    brls::Image* image;
    HTTP::Cancel isCancel;

    /// 对象池
    inline static std::list<Ref> pool;
    inline static std::mutex requestMutex;
    inline static std::unordered_map<brls::Image*, Ref> requests;
};