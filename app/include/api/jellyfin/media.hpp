#pragma once

#include <nlohmann/json.hpp>
#include <fmt/format.h>

namespace jellyfin {

const std::string apiUserViews = "/Users/{}/Views";
const std::string apiUserLibrary = "/Users/{}/Items?{}";
const std::string apiUserItem = "/Users/{}/Items/{}";
const std::string apiUserResume = "/Users/{}/Items/Resume?{}";
const std::string apiUserLatest = "/Users/{}/Items/Latest?{}";
const std::string apiShowNextUp = "/Shows/NextUp?{}";
const std::string apiShowSeanon = "/Shows/{}/Seasons?{}";
const std::string apiShowEpisodes = "/Shows/{}/Episodes?{}";
const std::string apiPrimaryImage = "/Items/{}/Images/Primary?{}";
const std::string apiLogoImage = "/Items/{}/Images/Logo?{}";

const std::string apiPlayback = "/Items/{}/PlaybackInfo";
const std::string apiStream = "/Videos/{}/stream?{}";
const std::string apiPlayStart = "/Sessions/Playing";
const std::string apiPlayStop = "/Sessions/Playing/Stopped";
const std::string apiPlaying = "/Sessions/Playing/Progress";

const std::string imageTypePrimary = "Primary";
const std::string imageTypeLogo = "Logo";

const std::string mediaTypeFolder = "Folder";
const std::string mediaTypeSeries = "Series";
const std::string mediaTypeSeason = "Season";
const std::string mediaTypeEpisode = "Episode";
const std::string mediaTypeMovie = "Movie";

const std::string streamTypeVideo = "Video";
const std::string streamTypeAudio = "Audio";
const std::string streamTypeSubtitle = "Subtitle";

struct UserDataResult {
    bool IsFavorite = false;
    int PlayCount = 0;
    double PlayedPercentage = 0;
    bool Played = false;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(UserDataResult, IsFavorite, PlayCount, PlayedPercentage, Played);

struct MediaItem {
    std::string Id;
    std::string Name;
    std::string Type;
    std::map<std::string, std::string> ImageTags;
    bool IsFolder = false;
    long ProductionYear = 0;
    float CommunityRating = 0.0f;
    int IndexNumber = 0;
    int ParentIndexNumber = 0;
    UserDataResult UserData;

    const std::string Title() {
        if (this->Type == mediaTypeEpisode)
            return fmt::format("S{}E{} {}", this->ParentIndexNumber, this->IndexNumber, this->Name);
        if (this->ProductionYear > 0) return fmt::format("{} ({})", this->Name, this->ProductionYear);
        return this->Name;
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MediaItem, Id, Name, Type, ImageTags, IsFolder, UserData,
    ProductionYear, CommunityRating, IndexNumber, ParentIndexNumber);

struct MediaSeason : public MediaItem {
    long IndexNumber = 0;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MediaSeason, Id, Name, Type, ImageTags, IsFolder, IndexNumber);

struct MediaStream {
    std::string Codec;
    std::string DisplayTitle;
    std::string Type;
    long Index = 0;
    bool IsDefault = false;
    bool IsExternal = false;
    std::string DeliveryUrl;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
    MediaStream, Codec, DisplayTitle, Type, Index, IsDefault, IsExternal, DeliveryUrl);

struct MediaSource {
    std::string Id;
    std::string Name;
    std::string Container;
    int DefaultAudioStreamIndex;
    int DefaultSubtitleStreamIndex;
    std::string ETag;
    time_t RunTimeTicks;
    std::vector<MediaStream> MediaStreams;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MediaSource, Id, Name, Container, DefaultAudioStreamIndex,
    DefaultSubtitleStreamIndex, ETag, RunTimeTicks, MediaStreams);

struct PlaybackResult {
    std::vector<MediaSource> MediaSources;
    std::string PlaySessionId;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlaybackResult, MediaSources, PlaySessionId);

struct MediaEpisode : public MediaSeason {
    std::string Overview;
    std::string SeriesName;
    std::string SeriesPrimaryImageTag;
    std::vector<MediaSource> MediaSources;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MediaEpisode, Id, Name, Type, ImageTags, IsFolder, Overview,
    IndexNumber, ParentIndexNumber, MediaSources, UserData, SeriesName, SeriesPrimaryImageTag);

template <typename T>
struct MediaQueryResult {
    std::vector<T> Items;
    long TotalRecordCount = 0;
    long StartIndex = 0;
};

template <typename T>
inline void to_json(nlohmann::json& nlohmann_json_j, const MediaQueryResult<T>& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, Items, TotalRecordCount, StartIndex))
}

template <typename T>
inline void from_json(const nlohmann::json& nlohmann_json_j, MediaQueryResult<T>& nlohmann_json_t) {
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, Items, TotalRecordCount, StartIndex))
}

}  // namespace jellyfin