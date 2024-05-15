#include "BoxTSTObjectCreate.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <string>

namespace eprosima {
namespace fastdds {
namespace dds {

class AmiSharePublisher
{
public:
AmiSharePublisher() 
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , type_(nullptr)
{}
bool create_publisher(void* type, std::string topic_name, std::string topic_type);
bool publish(void* data);
private:
  eprosima::fastdds::dds::DomainParticipant* participant_;
  eprosima::fastdds::dds::Publisher* publisher_;
  eprosima::fastdds::dds::Topic* topic_;
  eprosima::fastdds::dds::DataWriter* writer_;
  eprosima::fastdds::dds::TypeSupport type_;
  class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        PubListener()
            : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
            }
        }
        
        int matched_;
    }
    listener_;
};

} // namespace dds
} // namespace fastdds
} // namespace eprosima
