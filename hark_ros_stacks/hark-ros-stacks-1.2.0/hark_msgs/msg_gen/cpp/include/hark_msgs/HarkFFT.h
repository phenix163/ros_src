/* Auto-generated by genmsg_cpp for file /home/yhirai/catkin_ws/src/hark_ros_stacks/hark-ros-stacks-1.2.0/hark_msgs/msg/HarkFFT.msg */
#ifndef HARK_MSGS_MESSAGE_HARKFFT_H
#define HARK_MSGS_MESSAGE_HARKFFT_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "std_msgs/Header.h"
#include "hark_msgs/HarkFFTVal.h"

namespace hark_msgs
{
template <class ContainerAllocator>
struct HarkFFT_ {
  typedef HarkFFT_<ContainerAllocator> Type;

  HarkFFT_()
  : header()
  , count(0)
  , nch(0)
  , length(0)
  , src()
  {
  }

  HarkFFT_(const ContainerAllocator& _alloc)
  : header(_alloc)
  , count(0)
  , nch(0)
  , length(0)
  , src(_alloc)
  {
  }

  typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
   ::std_msgs::Header_<ContainerAllocator>  header;

  typedef int32_t _count_type;
  int32_t count;

  typedef int32_t _nch_type;
  int32_t nch;

  typedef int32_t _length_type;
  int32_t length;

  typedef std::vector< ::hark_msgs::HarkFFTVal_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::hark_msgs::HarkFFTVal_<ContainerAllocator> >::other >  _src_type;
  std::vector< ::hark_msgs::HarkFFTVal_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::hark_msgs::HarkFFTVal_<ContainerAllocator> >::other >  src;


  typedef boost::shared_ptr< ::hark_msgs::HarkFFT_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::hark_msgs::HarkFFT_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct HarkFFT
typedef  ::hark_msgs::HarkFFT_<std::allocator<void> > HarkFFT;

typedef boost::shared_ptr< ::hark_msgs::HarkFFT> HarkFFTPtr;
typedef boost::shared_ptr< ::hark_msgs::HarkFFT const> HarkFFTConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::hark_msgs::HarkFFT_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::hark_msgs::HarkFFT_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace hark_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::hark_msgs::HarkFFT_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::hark_msgs::HarkFFT_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::hark_msgs::HarkFFT_<ContainerAllocator> > {
  static const char* value() 
  {
    return "8aab5c2ab0dcabc23cdf16e2742a6c6d";
  }

  static const char* value(const  ::hark_msgs::HarkFFT_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x8aab5c2ab0dcabc2ULL;
  static const uint64_t static_value2 = 0x3cdf16e2742a6c6dULL;
};

template<class ContainerAllocator>
struct DataType< ::hark_msgs::HarkFFT_<ContainerAllocator> > {
  static const char* value() 
  {
    return "hark_msgs/HarkFFT";
  }

  static const char* value(const  ::hark_msgs::HarkFFT_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::hark_msgs::HarkFFT_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Header header\n\
int32 count\n\
int32 nch\n\
int32 length\n\
HarkFFTVal[] src # FFT signal of each channel\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.secs: seconds (stamp_secs) since epoch\n\
# * stamp.nsecs: nanoseconds since stamp_secs\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: hark_msgs/HarkFFTVal\n\
float32[] fftdata_real\n\
float32[] fftdata_imag\n\
\n\
";
  }

  static const char* value(const  ::hark_msgs::HarkFFT_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct HasHeader< ::hark_msgs::HarkFFT_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct HasHeader< const ::hark_msgs::HarkFFT_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::hark_msgs::HarkFFT_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.header);
    stream.next(m.count);
    stream.next(m.nch);
    stream.next(m.length);
    stream.next(m.src);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct HarkFFT_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::hark_msgs::HarkFFT_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::hark_msgs::HarkFFT_<ContainerAllocator> & v) 
  {
    s << indent << "header: ";
s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "count: ";
    Printer<int32_t>::stream(s, indent + "  ", v.count);
    s << indent << "nch: ";
    Printer<int32_t>::stream(s, indent + "  ", v.nch);
    s << indent << "length: ";
    Printer<int32_t>::stream(s, indent + "  ", v.length);
    s << indent << "src[]" << std::endl;
    for (size_t i = 0; i < v.src.size(); ++i)
    {
      s << indent << "  src[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::hark_msgs::HarkFFTVal_<ContainerAllocator> >::stream(s, indent + "    ", v.src[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // HARK_MSGS_MESSAGE_HARKFFT_H

