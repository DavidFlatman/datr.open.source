// Unclassified, OPEN SOURCE
#include "lib_irig106_ds.h"

namespace lib {
namespace irig106 {
namespace ds {

std::string toString(DATA_TYPE_NAME dtn)
{
    switch (dtn) {
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_0:  return "comp0";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_1:  return "setup";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_2:  return "event";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_3:  return "index";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_4:  return "comp4";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_5:  return "comp5";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_6:  return "comp6";
        case DTN_COMPUTER_GENERATED_DATA_FORMAT_7:  return "comp7";

        case DTN_PCM_DATA_FORMAT_0:                 return "pcm-0";
        case DTN_PCM_DATA_FORMAT_1:                 return "pcm-1";
        case DTN_PCM_DATA_FORMAT_2:                 return "pcm-2";
        case DTN_PCM_DATA_FORMAT_3:                 return "pcm-3";
        case DTN_PCM_DATA_FORMAT_4:                 return "pcm-4";
        case DTN_PCM_DATA_FORMAT_5:                 return "pcm-5";
        case DTN_PCM_DATA_FORMAT_6:                 return "pcm-6";
        case DTN_PCM_DATA_FORMAT_7:                 return "pcm-7";

        case DTN_TIME_DATA_FORMAT_0:                return "time1";
        case DTN_TIME_DATA_FORMAT_1:                return "time2";
        case DTN_TIME_DATA_FORMAT_2:                return "time3";
        case DTN_TIME_DATA_FORMAT_3:                return "time4";
        case DTN_TIME_DATA_FORMAT_4:                return "time5";
        case DTN_TIME_DATA_FORMAT_5:                return "time6";
        case DTN_TIME_DATA_FORMAT_6:                return "time7";
        case DTN_TIME_DATA_FORMAT_7:                return "time8";

        case DTN_MIL_STD_1553_DATA_FORMAT_0:        return "15530";
        case DTN_MIL_STD_1553_DATA_FORMAT_1:        return "1553B";
        case DTN_MIL_STD_1553_DATA_FORMAT_2:        return "15532";
        case DTN_MIL_STD_1553_DATA_FORMAT_3:        return "15533";
        case DTN_MIL_STD_1553_DATA_FORMAT_4:        return "15534";
        case DTN_MIL_STD_1553_DATA_FORMAT_5:        return "15535";
        case DTN_MIL_STD_1553_DATA_FORMAT_6:        return "15536";
        case DTN_MIL_STD_1553_DATA_FORMAT_7:        return "15537";

        case DTN_ANALOG_DATA_FORMAT_0:              return "alog0";
        case DTN_ANALOG_DATA_FORMAT_1:              return "alog1";
        case DTN_ANALOG_DATA_FORMAT_2:              return "alog2";
        case DTN_ANALOG_DATA_FORMAT_3:              return "alog3";
        case DTN_ANALOG_DATA_FORMAT_4:              return "alog4";
        case DTN_ANALOG_DATA_FORMAT_5:              return "alog5";
        case DTN_ANALOG_DATA_FORMAT_6:              return "alog6";
        case DTN_ANALOG_DATA_FORMAT_7:              return "alog7";

        case DTN_DISCRETE_DATA_FORMAT_0:            return "disc0";
        case DTN_DISCRETE_DATA_FORMAT_1:            return "disc1";
        case DTN_DISCRETE_DATA_FORMAT_2:            return "disc2";
        case DTN_DISCRETE_DATA_FORMAT_3:            return "disc3";
        case DTN_DISCRETE_DATA_FORMAT_4:            return "disc4";
        case DTN_DISCRETE_DATA_FORMAT_5:            return "disc5";
        case DTN_DISCRETE_DATA_FORMAT_6:            return "disc6";
        case DTN_DISCRETE_DATA_FORMAT_7:            return "disc7";

        case DTN_MESSAGE_DATA_FORMAT_0:             return "msg-0";
        case DTN_MESSAGE_DATA_FORMAT_1:             return "msg-1";
        case DTN_MESSAGE_DATA_FORMAT_2:             return "msg-2";
        case DTN_MESSAGE_DATA_FORMAT_3:             return "msg-3";
        case DTN_MESSAGE_DATA_FORMAT_4:             return "msg-4";
        case DTN_MESSAGE_DATA_FORMAT_5:             return "msg-5";
        case DTN_MESSAGE_DATA_FORMAT_6:             return "msg-6";
        case DTN_MESSAGE_DATA_FORMAT_7:             return "msg-7";

        case DTN_ARINC_429_DATA_FORMAT_0:           return "arin0";
        case DTN_ARINC_429_DATA_FORMAT_1:           return "arin1";
        case DTN_ARINC_429_DATA_FORMAT_2:           return "arin2";
        case DTN_ARINC_429_DATA_FORMAT_3:           return "arin3";
        case DTN_ARINC_429_DATA_FORMAT_4:           return "arin4";
        case DTN_ARINC_429_DATA_FORMAT_5:           return "arin5";
        case DTN_ARINC_429_DATA_FORMAT_6:           return "arin6";
        case DTN_ARINC_429_DATA_FORMAT_7:           return "arin7";

        case DTN_VIDEO_DATA_FORMAT_0:               return "vid-0";
        case DTN_VIDEO_DATA_FORMAT_1:               return "vid-1";
        case DTN_VIDEO_DATA_FORMAT_2:               return "vid-2";
        case DTN_VIDEO_DATA_FORMAT_3:               return "vid-3";
        case DTN_VIDEO_DATA_FORMAT_4:               return "vid-4";
        case DTN_VIDEO_DATA_FORMAT_5:               return "vid-5";
        case DTN_VIDEO_DATA_FORMAT_6:               return "vid-6";
        case DTN_VIDEO_DATA_FORMAT_7:               return "vid-7";

        case DTN_IMAGE_DATA_FORMAT_0:               return "img-0";
        case DTN_IMAGE_DATA_FORMAT_1:               return "img-1";
        case DTN_IMAGE_DATA_FORMAT_2:               return "img-2";
        case DTN_IMAGE_DATA_FORMAT_3:               return "img-3";
        case DTN_IMAGE_DATA_FORMAT_4:               return "img-4";
        case DTN_IMAGE_DATA_FORMAT_5:               return "img-5";
        case DTN_IMAGE_DATA_FORMAT_6:               return "img-6";
        case DTN_IMAGE_DATA_FORMAT_7:               return "img-7";

        case DTN_UART_DATA_FORMAT_0:                return "uart0";
        case DTN_UART_DATA_FORMAT_1:                return "uart1";
        case DTN_UART_DATA_FORMAT_2:                return "uart2";
        case DTN_UART_DATA_FORMAT_3:                return "uart3";
        case DTN_UART_DATA_FORMAT_4:                return "uart4";
        case DTN_UART_DATA_FORMAT_5:                return "uart5";
        case DTN_UART_DATA_FORMAT_6:                return "uart6";
        case DTN_UART_DATA_FORMAT_7:                return "uart7";

        case DTN_IEEE_1394:                         return "13940";
        case DTN_IEEE_1394_DATA_FORMAT_1:           return "13941";
        case DTN_IEEE_1394_DATA_FORMAT_2:           return "13942";
        case DTN_IEEE_1394_DATA_FORMAT_3:           return "13943";
        case DTN_IEEE_1394_DATA_FORMAT_4:           return "13944";
        case DTN_IEEE_1394_DATA_FORMAT_5:           return "13945";
        case DTN_IEEE_1394_DATA_FORMAT_6:           return "13946";
        case DTN_IEEE_1394_DATA_FORMAT_7:           return "13947";

        case DTN_PARALLEL_DATA_FORMAT_0:            return "par-0";
        case DTN_PARALLEL_DATA_FORMAT_1:            return "par-1";
        case DTN_PARALLEL_DATA_FORMAT_2:            return "par-2";
        case DTN_PARALLEL_DATA_FORMAT_3:            return "par-3";
        case DTN_PARALLEL_DATA_FORMAT_4:            return "par-4";
        case DTN_PARALLEL_DATA_FORMAT_5:            return "par-5";
        case DTN_PARALLEL_DATA_FORMAT_6:            return "par-6";
        case DTN_PARALLEL_DATA_FORMAT_7:            return "par-7";

        case DTN_ETHERNET_DATA_FORMAT_0:            return "eth-0";
        case DTN_ETHERNET_DATA_FORMAT_1:            return "eth-1";
        case DTN_ETHERNET_DATA_FORMAT_2:            return "eth-2";
        case DTN_ETHERNET_DATA_FORMAT_3:            return "eth-3";
        case DTN_ETHERNET_DATA_FORMAT_4:            return "eth-4";
        case DTN_ETHERNET_DATA_FORMAT_5:            return "eth-5";
        case DTN_ETHERNET_DATA_FORMAT_6:            return "eth-6";
        case DTN_ETHERNET_DATA_FORMAT_7:            return "eth-7";

        default:                                    return "error";

    }
} // std::string toString(DATA_TYPE_NAME dtn)

} // namespace ds //
} // namespace irig106 //
} // namespace lib //
