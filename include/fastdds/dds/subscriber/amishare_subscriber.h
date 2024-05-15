#include "BoxTSTObjectCreate.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <string>

namespace eprosima {
namespace fastdds {
namespace dds {

class AmiShareSubscriber
{
public:
AmiShareSubscriber() 
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(nullptr)
{}

using Function = void(*)(void*);
static Function subscriber_callback_; 
//static void (*subscriber_callback_)(void*); 

//AmiShareSubscriber(void (*f)(void*)) 
AmiShareSubscriber(Function f) 
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(nullptr)
{ 
  subscriber_callback_ = f;
}

bool create_subscriber(void* type, std::string topic_name, std::string topic_type);
//bool create_subscriber(void* type, std::string topic_name, std::string topic_type, void (*f)(void*));

private:
  eprosima::fastdds::dds::DomainParticipant* participant_;
  eprosima::fastdds::dds::Subscriber* subscriber_;
  eprosima::fastdds::dds::Topic* topic_;
  eprosima::fastdds::dds::DataReader* reader_;
  eprosima::fastdds::dds::TypeSupport type_;

  class SubListener : public eprosima::fastdds::dds::DataReaderListener
    {
    public:

        SubListener()
            : samples_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override
        {
        std::cout << "TEBD: data available\n";
        void *data;
        SampleInfo info;
        reader->amishare_take_next_sample(data, &info);
        //subscriber_callback_(static_cast<type_->type>(data));
        AmiShareSubscriber::subscriber_callback_(data);

                    //samples_++;
                    //const size_t data_size = hello_->data().size();
                    //std::cout << "Message " << hello_->message() << " " << hello_->index
                            //<< " RECEIVED With " << data_size << "(bytes) of Data. Dat
                            //<< (char*)&hello_->data()[data_size - 9] << std::endl;
        }
        
        int samples_;
    }
    listener_;

};

} // namespace dds
} // namespace fastdds
} // namespace eprosima
