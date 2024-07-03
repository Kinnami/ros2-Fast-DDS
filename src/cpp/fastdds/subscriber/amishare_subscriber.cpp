#include "fastdds/dds/subscriber/amishare_subscriber.h"
#include <iostream>

using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

namespace eprosima {
namespace fastdds {
namespace dds {

//bool AmiShareSubscriber::create_subscriber(void* type, std::string topic_name, std::string topic_type, void (*f)(void*))
bool AmiShareSubscriber::create_subscriber(void* message_value, void* type, std::string topic_name, std::string topic_type)
{
    data_ = message_value;

    //AmiShareSubscriber::subscriber_callback_ = f;
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SIMPLE;
    pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.name("test");

    //pqos.transport().use_builtin_transports = false;
    //auto shm_transport = std::make_shared<SharedMemTransportDescriptor>();
    //shm_transport->segment_size(10000);
    //shm_transport->max_message_size(10000);
    //pqos.transport().user_transports.push_back(shm_transport);
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
    if (participant_ == nullptr)
        return false;

    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    if (subscriber_ == nullptr)
        return false;

    type_ = TypeSupport(static_cast<TopicDataType*>(type));
    if (!(type_.register_type(participant_, topic_type)))
        return false;

    topic_ = participant_->create_topic(topic_name, topic_type, TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr)
        return false;

    DataReaderQos rqos;
    rqos.history().kind = KEEP_LAST_HISTORY_QOS;
    rqos.history().depth = 30;
    rqos.resource_limits().max_samples = 50;
    rqos.resource_limits().allocated_samples = 20;
    rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    rqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;

    reader_ = subscriber_->create_datareader(topic_, rqos, &listener_, true); 
    if (reader_ == nullptr)
        return false;

    return true;
}

void* AmiShareSubscriber::subscriber_callback_;
void* AmiShareSubscriber::data_;

/*
void AmiShareSubscriber::SubListener::on_data_available(eprosima::fastdds::dds::DataReader* reader) 
{
        void *data;
        SampleInfo info;
        reader->amishare_take_next_sample(data, &info);
        //subscriber_callback_(static_cast<type_->type>(data));
        AmiShareSubscriber::subscriber_callback_(data);
}
*/

} // namespace dds
} // namespace fastdds
} // namespace eprosima

