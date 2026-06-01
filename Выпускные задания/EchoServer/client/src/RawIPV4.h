#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SIZE_DATA 255

/*!
 * \brief Структура пакета UDP на траспортном уровене
 */
struct Packet_UDP {
	uint16_t source_port;
	uint16_t destination_port;
	uint16_t length;
	uint16_t checksum;
	char data[SIZE_DATA];
};

/*!
 * \brief Струтура пакета UDP на сетевом уровне
 */
struct Packet_IPV4 {
	uint8_t ver_and_ihl;
	uint8_t ds;
	uint16_t total_length;
	uint16_t identification;
	uint16_t flags_and_offset;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t source_address;
	uint32_t destination_address;

	struct Packet_UDP packet_udp;
};

/*!
 * \brief Функция, отвечающая за заполнение UDP пакета на траспортном уровне
 * \param struct Packet_UDP* packet_udp - указатель на структуру пакета, который
 * будет заполняться
 * \param uint16_t port_source - потр отправителя
 * \param uint16_t port_destination - порт получателя
 * \param char* msg - сообщение
 */
void FillPacketUDP(struct Packet_UDP* packet_udp, uint16_t port_source,
		   uint16_t port_destination, char* msg);

/*!
 * \brief Функция, отвечающая за заполнение UDP пакета на сетевом уровне
 * \parma struct Packet_IPV4* packet_ipv4 - указатель на структуру заголовка,
 * который будет заполняться
 * \param uint32_t ip_addr_source - ipv4 адрес отправителя
 * \param uint32_t ip_addr_destination - ipv4 драс получателя
 * \param struct Packet_UDP* packet_udp - указатель на пакет udp
 */
void FillPacketIPV4(struct Packet_IPV4* packet_ipv4, uint32_t ip_addr_source,
		    uint32_t ip_addr_destination,
		    struct Packet_UDP* packet_udp);

/*!
 * \brief Функция, отвечающая за подчёт чексуммы
 * \param int count - количество байтов
 * \param void* addr - указатель на структуру пакета
 */
long CountChecksum(int count, void* addr);

/*!
 * \brief Функция, проверяющая, что поступивший пакет ipv4 действительно пришёл
 * по нужному адресу
 * \param struct Packet_IPV4* packet_ipv4 - указатель на
 * струтуру пакета ipv4, которй будет проверяться
 * \param uint32_t source_address - адресс отправителя
 * \param uint16_t source_port - порт отправителя
 * \return Число, обозначающее успех проверки. 1 - если адрес и порт получетеля
 * (packet_ipv4) совпадает с адресом и портом отправителя (source_address,
 * source_port). 0 - адреса и порты не совпадают.
 */
int PacketIPV4Check(struct Packet_IPV4* packet_ipv4, uint32_t source_address,
		    uint16_t source_port);
