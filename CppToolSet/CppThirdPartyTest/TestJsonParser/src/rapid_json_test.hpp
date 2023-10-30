
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

void RapidJsonTest()
{
    std::wifstream inputFile("./config/config.json");
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();
    
    using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
    using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;
    
    DocumentW doc;
    doc.Parse(fileContent.str().c_str());

    std::wcout << doc[L"tag1"].GetString() << std::endl;
    std::wcout << doc[L"tag2"].GetBool() << std::endl;

    const ValueW& arrayNode = doc[L"array1"];
    size_t arrLength = arrayNode.Size();
    for (int i = 0; i < arrLength; i++)
    {
        const ValueW& element = arrayNode[i];
        std::wcout << element[L"name"].GetString() << std::endl;
        std::wcout << element[L"desc"].GetString() << std::endl;
    }
}

void RapidJson5Test()
{
    std::wifstream inputFile("./config/config.json");
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
    using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;

    constexpr int Json5Parse = rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag;

    DocumentW doc;
    doc.Parse<Json5Parse>(fileContent.str().c_str());

    std::wcout << doc[L"tag1"].GetString() << std::endl;
    std::wcout << doc[L"tag2"].GetBool() << std::endl;

    const ValueW& arrayNode = doc[L"array1"];
    size_t arrLength = arrayNode.Size();
    for (int i = 0; i < arrLength; i++)
    {
        const ValueW& element = arrayNode[i];
        std::wcout << element[L"name"].GetString() << std::endl;
        std::wcout << element[L"desc"].GetString() << std::endl;
    }
}

typedef enum _HwcPipeCounterType
{
    AAA,
    BBB
} HwcPipeCounterType;

struct HwcPipeSample {
    /** @brief Type of the sampled data. */
    enum class type { uint64, float64 };

    /** @brief Value of the sample. */
    union value {
        uint64_t uint64;
        double float64;

        explicit value(uint64_t int_v)
                : uint64(int_v) {}

        explicit value(double double_v)
                : float64(double_v) {}
    };

    /** The counter id. */
    HwcPipeCounterType counter;
    /** The timestamp of the sample. */
    uint64_t timestamp;
    /** The sample value (variant is defined by the type field). */
    value value;
    /** The data type of the sampled value. */
    type type;

    HwcPipeSample(HwcPipeCounterType counter, uint64_t timestamp, uint64_t value)
            : counter(counter)
            , timestamp(timestamp)
            , value(value)
            , type(type::uint64) {}

    HwcPipeSample(HwcPipeCounterType counter, uint64_t timestamp, double value)
            : counter(counter)
            , timestamp(timestamp)
            , value(value)
            , type(type::float64) {}
};

void RapidJsonGen()
{
    using RapidUtf8 = rapidjson::UTF8<>;
    using RapidJsonDoc = rapidjson::GenericDocument<RapidUtf8>;
    using RapidJsonValue = rapidjson::GenericValue<RapidUtf8>;
    using RapidJsonStringBuffer = rapidjson::GenericStringBuffer<RapidUtf8>;
    using RapidJsonPrettyWriter = rapidjson::PrettyWriter<RapidJsonStringBuffer, RapidUtf8, RapidUtf8>;

    static std::unordered_map<HwcPipeCounterType, std::vector<HwcPipeSample>> gCounterSampleMap;

    gCounterSampleMap.clear();

    gCounterSampleMap[AAA].push_back(HwcPipeSample(AAA, (uint64_t)1233, (uint64_t)55));
    gCounterSampleMap[AAA].push_back(HwcPipeSample(AAA, (uint64_t)1233, (uint64_t)123));
    gCounterSampleMap[AAA].push_back(HwcPipeSample(AAA, (uint64_t)1233, (uint64_t)53));
    gCounterSampleMap[AAA].push_back(HwcPipeSample(AAA, (uint64_t)1233, (uint64_t)5123));

    gCounterSampleMap[BBB].push_back(HwcPipeSample(BBB, (uint64_t)1233, 123.0f));
    gCounterSampleMap[BBB].push_back(HwcPipeSample(BBB, (uint64_t)1233, 134523.0f));
    gCounterSampleMap[BBB].push_back(HwcPipeSample(BBB, (uint64_t)1233, 12563.0f));
    gCounterSampleMap[BBB].push_back(HwcPipeSample(BBB, (uint64_t)1233, 15623.0f));

    std::fstream fs("./test.json", std::ios::out);
    if (!fs.is_open())
        return 1;

    RapidJsonDoc doc;
    doc.SetArray();

    for (auto& [counterType, sampleVec] : gCounterSampleMap)
    {
        RapidJsonValue counterObj(rapidjson::kObjectType);

        counterObj.AddMember("CounterId", RapidJsonValue(counterType), doc.GetAllocator());

        RapidJsonValue dataArr(rapidjson::kArrayType);

        for (auto& data : sampleVec)
        {
            RapidJsonValue dataObj(rapidjson::kObjectType);

            dataObj.AddMember("TimeStamp", RapidJsonValue(data.timestamp), doc.GetAllocator());

            if (data.type == HwcPipeSample::type::uint64)
            {
                RapidJsonValue dataUInt64Obj(rapidjson::kObjectType);
                dataUInt64Obj.AddMember("$type", RapidJsonValue("DataUint64"), doc.GetAllocator());
                dataUInt64Obj.AddMember("value", RapidJsonValue(data.value.uint64), doc.GetAllocator());
                dataObj.AddMember("data", dataUInt64Obj, doc.GetAllocator());
            }
            else if (data.type == HwcPipeSample::type::float64)
            {
                RapidJsonValue dataFloat64Obj(rapidjson::kObjectType);
                dataFloat64Obj.AddMember("$type", RapidJsonValue("DataDouble"), doc.GetAllocator());
                dataFloat64Obj.AddMember("value", RapidJsonValue(data.value.float64), doc.GetAllocator());
                dataObj.AddMember("data", dataFloat64Obj, doc.GetAllocator());
            }

            dataArr.PushBack(dataObj, doc.GetAllocator());
        }

        counterObj.AddMember("Data", dataArr, doc.GetAllocator());
        doc.PushBack(counterObj, doc.GetAllocator());
    }

    RapidJsonStringBuffer sb;
    RapidJsonPrettyWriter writer{ sb };

    doc.Accept(writer);

    fs << sb.GetString();
    fs.close();
}