#ifndef _SESSION_PARSER_HPP_
#define _SESSION_PARSER_HPP_

#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <StdFileSystem.hpp>

namespace trinity {

class SessionParser : public std::enable_shared_from_this<SessionParser> {
public:
    /*** type define ***/
 using OnUnpackedListener = std::function<void(const std::vector<uint8_t>& headData,
                                          const std::filesystem::path& bodyPath)>;

 /*** static function and variable ***/

 /*** class function and variable ***/
 explicit SessionParser() = default;
 virtual ~SessionParser() = default;

 void config(const std::filesystem::path& cacheDir);

 int unpack(const std::vector<uint8_t>& data,
            std::shared_ptr<OnUnpackedListener> listener);
 int pack(std::vector<uint8_t>& data,
          const std::vector<uint8_t>& headData,
          const std::filesystem::path& bodyPath);

protected:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/

private:
    /*** type define ***/
    struct Protocol {
        struct Info {
            int64_t magicNumber;
            uint32_t version;
            int32_t headSize;
            int64_t bodySize;
        };
        struct Payload {
            explicit Payload(const std::filesystem::path& bodyPath);
            virtual ~Payload();
            std::vector<uint8_t> headData;
            struct {
                std::filesystem::path filepath;
                std::fstream stream;
                int64_t receivedBodySize;
            } bodyData;
        };

        Info info;
        std::unique_ptr<Payload> payload;

    private:
        static constexpr const int64_t MagicNumber = 0x00A5202008275A;
        static constexpr const uint32_t Version_01_00_00 = 10000;

        friend SessionParser;
    };

    /*** static function and variable ***/
    static constexpr const char* BodyCacheName = "session-bodydata-";

    /*** class function and variable ***/
    int unpackProtocol(const std::vector<uint8_t>& data, int offset);
    int unpackBodyData(const std::vector<uint8_t>& data, int offset,
                       std::shared_ptr<OnUnpackedListener> listener);
    int64_t ntoh(int64_t value) const;
    int64_t hton(int64_t value) const;
    int32_t ntoh(int32_t value) const;
    int32_t hton(int32_t value) const;
    uint32_t ntoh(uint32_t value) const;
    uint32_t hton(uint32_t value) const;
    // reserved
    // uint16_t ntoh(uint16_t value) const;
    // uint16_t hton(uint16_t value) const;

    std::filesystem::path bodyCacheDir;

    std::unique_ptr<Protocol> protocol;
    std::vector<uint8_t> cachingData;
};

/***********************************************/
/***** class template function implement *******/
/***********************************************/

/***********************************************/
/***** macro definition ************************/
/***********************************************/

} // namespace trinity

#endif /* _SESSION_PARSER_HPP_ */


