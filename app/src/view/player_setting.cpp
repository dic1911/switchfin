#include "view/player_setting.hpp"
#include "view/mpv_core.hpp"
#include "utils/config.hpp"

using namespace brls::literals;

PlayerSetting::PlayerSetting(const jellyfin::MediaSource& src) {
    this->inflateFromXMLRes("xml/view/player_setting.xml");
    brls::Logger::debug("PlayerSetting: create");

    this->registerAction("hints/cancel"_i18n, brls::BUTTON_B, [](...) {
        brls::Application::popActivity();
        return true;
    });

    std::vector<std::string> subtitleOption, audioOption;
    subtitleOption.push_back("main/player/none"_i18n);
    for (auto& it : src.MediaStreams) {
        if (it.Type == jellyfin::streamTypeSubtitle) {
            subtitleOption.push_back(it.DisplayTitle);
        } else if (it.Type == jellyfin::streamTypeAudio) {
            audioOption.push_back(it.DisplayTitle);
        }
    }
    // 字幕选择
    if (subtitleOption.size() > 1) {
        int64_t value = 0;
        MPVCore::instance().get_property("sid", MPV_FORMAT_INT64, &value);
        this->subtitleTrack->init("main/player/subtitle"_i18n, subtitleOption, value,
            [](int selected) { MPVCore::instance().set_property("sid", selected); });
        this->subtitleTrack->detail->setVisibility(brls::Visibility::GONE);
    } else {
        this->subtitleTrack->setVisibility(brls::Visibility::GONE);
    }
    // 音轨选择
    if (audioOption.size() > 1) {
        int64_t value = 0;
        if (!MPVCore::instance().get_property("aid", MPV_FORMAT_INT64, &value)) value -= 1;
        this->audioTrack->init("main/player/audio"_i18n, audioOption, value,
            [](int selected) { MPVCore::instance().set_property("aid", selected + 1); });
        this->audioTrack->detail->setVisibility(brls::Visibility::GONE);
    } else {
        this->audioTrack->setVisibility(brls::Visibility::GONE);
    }

    auto& conf = AppConfig::instance();
    auto& seekingOption = conf.getOptions(AppConfig::PLAYER_SEEKING_STEP);
    seekingStep->init("main/setting/playback/seeking_step"_i18n, seekingOption.options,
        conf.getValueIndex(AppConfig::PLAYER_SEEKING_STEP, 2), [&seekingOption](int selected) {
            MPVCore::SEEKING_STEP = seekingOption.values[selected];
            AppConfig::instance().setItem(AppConfig::PLAYER_SEEKING_STEP, MPVCore::SEEKING_STEP);
        });

/// Fullscreen
#if defined(__linux__) || defined(_WIN32)
    btnFullscreen->init(
        "main/setting/others/fullscreen"_i18n, conf.getItem(AppConfig::FULLSCREEN, false), [](bool value) {
            AppConfig::instance().setItem(AppConfig::FULLSCREEN, value);
            // 设置当前状态
            brls::Application::getPlatform()->getVideoContext()->fullScreen(value);
        });
#else
    btnFullscreen->setVisibility(brls::Visibility::GONE);
#endif
}

PlayerSetting::~PlayerSetting() { brls::Logger::debug("PlayerSetting: delete"); }
