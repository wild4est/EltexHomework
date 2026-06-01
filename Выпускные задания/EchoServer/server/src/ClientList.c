#include "ClientList.h"

struct List* InitList(){
	struct List* list = malloc(sizeof(struct List));
	list->length = 0;
	list->start = NULL;
}

void InitClient(struct Client* client, struct sockaddr_in* addr){
	memcpy(&(client->addr), addr, sizeof(*addr));
	client->msg_count = 1;
}

void AddNode(struct List* list, struct Client* client){
	struct Node** cur_node = &(list->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct Node));
	memcpy(&((*cur_node)->client), client, sizeof(*client));
	(*cur_node)->next = NULL;
	list->length++;
}

int CountLength(struct List* list){
	int n = 0;
	struct Node* cur_node = list->start;
	while(cur_node != NULL){
		n++;
		cur_node = cur_node->next;
	}
	return n;

}

struct Client* SearchClient(struct List* list, struct sockaddr_in* new_addr){
	struct Node* cur_node = list->start;
	while(cur_node != NULL) {
		uint32_t old_client_addr = cur_node->client.addr.sin_addr.s_addr;
		uint32_t new_client_addr = new_addr->sin_addr.s_addr;

		uint16_t old_client_port = cur_node->client.addr.sin_port;
		uint16_t new_client_port = new_addr->sin_port;
		
		if (old_client_addr == new_client_addr && old_client_port == new_client_port) {
			return &(cur_node->client);
		}
		cur_node = cur_node->next;
	}
	return NULL;
}

void DeleteNodeByAddr(struct List* list, struct sockaddr_in* addr){
	if (list->start == NULL) {
		return;
	}

	struct Node* delete_node;
	
	uint32_t ip_addr_1 = list->start->client.addr.sin_addr.s_addr;
	uint16_t port_1 = list->start->client.addr.sin_port;
	uint32_t ip_addr_2 = addr->sin_addr.s_addr;
	uint16_t port_2 = addr->sin_port;

	if (PortAndAddrCheck( ip_addr_1, port_1 , ip_addr_2, port_2)) {
		delete_node = list->start;
		list->start = list->start->next;
		free(delete_node);
		list->length--;
		return;
	}

	struct Node* cur_node = list->start;
		
	while(cur_node->next != NULL){
		ip_addr_1 = cur_node->next->client.addr.sin_addr.s_addr;
		port_1 = cur_node->next->client.addr.sin_port;
		if ( PortAndAddrCheck(ip_addr_1, port_1, ip_addr_2, port_2 ) ) {
			break;
		}
		cur_node = cur_node->next;
		
	}
	delete_node = cur_node->next;
	if (delete_node == NULL) {
		return;
	}
	
	printf("Удалён пользователь[ addr = %X | port = %d]\n", delete_node->client.addr.sin_addr.s_addr, delete_node->client.addr.sin_port);
	cur_node->next = cur_node->next->next;
	free(delete_node);
	list->length--;
}


void FreeList(struct List* list){
        struct Node* cur_node = list->start;
        while(cur_node != NULL){
                struct Node* delete_node = cur_node;
                cur_node = cur_node->next;
                free(delete_node);
        }
        free(list);
}

