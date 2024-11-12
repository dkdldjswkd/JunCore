#include <boost/asio/deadline_timer.hpp>

using DeadlineTimer = boost::asio::basic_deadline_timer<boost::posix_time::ptime, boost::asio::time_traits<boost::posix_time::ptime>, boost::asio::io_context::executor_type>;