//
// Created by fang on 2023/3/4.
//

#pragma once

#include <borealis.hpp>
#include <api/jellyfin/media.hpp>

class ButtonClose;

class PlayerSetting : public brls::Box {
public:
    PlayerSetting(const jellyfin::MediaSource& src);
    ~PlayerSetting() override;

    bool isTranslucent() override { return true; }

    View* getDefaultFocus() override { return this->settings->getDefaultFocus(); }

    inline static int selectedSubtitle = 0;
    inline static int selectedAudio = 0;

private:
    BRLS_BIND(brls::ScrollingFrame, settings, "player/settings");
    BRLS_BIND(brls::Box, cancel, "player/cancel");

    BRLS_BIND(brls::SelectorCell, subtitleTrack, "setting/track/subtitle");
    BRLS_BIND(brls::SelectorCell, audioTrack, "setting/track/audio");
    BRLS_BIND(brls::SelectorCell, seekingStep, "setting/player/seeking");
    BRLS_BIND(brls::BooleanCell, btnBottomBar, "setting/player/bottom_bar");
    BRLS_BIND(brls::BooleanCell, btnOSDOnToggle, "setting/player/osd_on_toggle");
    BRLS_BIND(brls::BooleanCell, btnFullscreen, "setting/fullscreen");
    BRLS_BIND(brls::BooleanCell, btnAlwaysOnTop, "setting/always_on_top");
    BRLS_BIND(brls::SelectorCell, btnVideoMirror, "setting/video/mirror");
    BRLS_BIND(brls::SelectorCell, btnVideoAspect, "setting/video/aspect");
    BRLS_BIND(brls::SliderCell, btnSubsync, "setting/video/subsync");
};