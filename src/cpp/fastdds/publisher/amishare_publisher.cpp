#include "fastdds/dds/publisher/amishare_publisher.h"
#include <iostream>

using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

namespace eprosima {
namespace fastdds {
namespace dds {

bool AmiSharePublisher::create_publisher(void* type, std::string topic_name, std::string topic_type)
{
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SIMPLE;
    pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.name("test");

    pqos.transport().use_builtin_transports = false;
    auto shm_transport = std::make_shared<SharedMemTransportDescriptor>();
    shm_transport->segment_size(10000);
    shm_transport->max_message_size(10000);
    pqos.transport().user_transports.push_back(shm_transport);
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
    if (participant_ == nullptr)
        return false;

    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);
    if (publisher_ == nullptr)
        return false;

    type_ = TypeSupport(static_cast<TopicDataType*>(type));
    if (!(type_.register_type(participant_, topic_type)))
        return false;

    topic_ = participant_->create_topic(topic_name, topic_type, TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr)
        return false;

    DataWriterQos wqos;
    wqos.history().kind = KEEP_LAST_HISTORY_QOS;
    wqos.history().depth = 30;
    wqos.resource_limits().max_samples = 50;
    wqos.resource_limits().allocated_samples = 20;
    wqos.reliable_writer_qos().times.heartbeatPeriod.seconds = 2;
    wqos.reliable_writer_qos().times.heartbeatPeriod.nanosec = 200 * 1000 * 1000;
    wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    wqos.publish_mode().kind = ASYNCHRONOUS_PUBLISH_MODE;

    writer_ = publisher_->create_datawriter(topic_, wqos, &listener_, true); 
    if (writer_ == nullptr)
        return false;

    return true;
}

bool AmiSharePublisher::publish(void* data)
{
    return writer_->amishare_write(data);
}

} // namespace dds
} // namespace fastdds
} // namespace eprosima

