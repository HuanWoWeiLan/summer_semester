/************************************************************************

单链表

FUNCTIONS :
ListCreate			- 创建一个新的链表。
ListInsertAt			- 插入节点到链表。
ListPushFront		- 在链表头部插入节点。
ListPushBack			- 在链表尾部插入节点。
ListPopFront			- 删除链表头部节点。
ListPopBack			- 删除链表尾部节点。
ListGetFront			- 获得第一个节点
ListGetBack			- 获得最后一个节点。
ListGetNodeAt		- 根据序号获得节点。
ListDeleteNodeAt		- 根据序号删除节点。
ListDestroy			- 删除整个链表。
ListDestroyAndFree	- 删除整个链表，并释放数据。
ListClear			- 删除链表中的所有节点。
ListClearAndFree		- 清除链表中的所有节点，并释放数据。
ListGetSize			- 获得当前链表大小（节点数量）。


************************************************************************/
#include <stdlib.h> // 使用到了malloc(), free()等函数。
#include "LinkList.h"



#ifdef _LIST_DEBUG
#include <stdio.h>
void ListPrint(PLIST list)
{
	int i;
	int n = ListSize(list);
	printf("size: %d\n", ListSize(list));
	for (i = 0; i < n; i++)
	{
		printf("data: %p", ListGetAt(list, i));
	}
}

int test_case()
{
	PLIST test = ListCreate(0);
	ListPrint(test);

	ListPushBack(test, 0);
	ListPrint(test);

	ListPushBack(test, 1);
	ListPushBack(test, 2);
	ListPushBack(test, 3);
	ListPrint(test);

	ListPushBack(test, 4);
	ListPushBack(test, 5);
	ListPushBack(test, 6);
	ListPrint(test);

	ListDeleteAt(test, 4);
	ListPrint(test);

	ListDeleteAt(test, 0);
	ListPrint(test);

	ListDeleteAt(test, 5);
	ListPrint(test);

	ListInsertAt(test, 3, 7);
	ListPrint(test);

	ListInsertAt(test, 0, 8);
	ListPrint(test);

	ListInsertAt(test, ListSize(test) - 1, 9);
	ListPrint(test);

	ListDistory(test);
}

#endif


// 创建一个链表节点
PNODE NodeCreate(void* data)
{
	PNODE pnode = (PNODE)malloc(sizeof(NODE));
	pnode->data = data;
	pnode->next = 0;
	return pnode;
}

// 销毁链表节点及其数据。
void NodeDistory(PNODE node)
{
	free(node->data);
	free(node);
}

// 获得链表的大小（节点数量）。
int ListSize(PLIST list)
{
	int  cnt;
	PNODE node = list->first;

	cnt = 0;
	while (node)
	{
		cnt++;
		node = node->next;
	}
	return (cnt);
}

// 删除链表中特定位置的节点，并返回数据。
// 返回 -1 表示错误，
// 返回 >= 表示插入的位置。
int ListInsertAt(PLIST list, unsigned int n, void* data)
{
	unsigned int  i;
	PNODE tmp;
	PNODE prev;
	PNODE new_node;

	if (n < 0)
		return -1;
	if (n == 0)
	{
		ListPushFront(list, data);
		return 0;
	}
	i = 1;
	tmp = list->first;
	prev = tmp;
	while (i <= n)
	{
		prev = tmp;
		tmp = tmp->next;
		if (!tmp)
			return -1;
		++i;
	}
	new_node = NodeCreate(data);
	prev->next = new_node;
	new_node->next = tmp;
	return i;
}


// 删除链表中特定位置的节点，并返回数据。
void* ListDeleteAt(PLIST list, int n)
{
	int  i;
	PNODE tmp;
	PNODE prev;
	void* data;

	tmp = list->first;
	prev = tmp;
	if (n < 0)
		return (NULL);
	if (n == 0)
	{
		return ListPopFront(list);
	}
	i = 1;

	while (i <= n)
	{
		prev = tmp;
		tmp = tmp->next;
		if (!tmp)
			return (NULL);
		++i;
	}
	prev->next = tmp->next;
	data = tmp->data;
	free(tmp);
	return data;
}


// 获得指定位置上节点中的数据。
void* ListGetAt(PLIST list, unsigned int n)
{
	unsigned int   i;
	PNODE node = list->first;

	if (!node)
		return (NULL);

	for (i = 1; i <= n; i++)
	{
		node = node->next;
		if (!node)
			return (NULL);
	}
	return (node->data);
}

// 创建链表。
PLIST ListCreate(void* list_data)
{
	PLIST new_list;

	new_list = NULL;
	new_list = (PLIST)malloc(sizeof(LIST));
	if (new_list != NULL)
	{
		new_list->list_data = list_data;
		new_list->first = NULL;
	}
	return (new_list);
}

// 只释放链表头，而不释放其他。
void ListDistory(PLIST list)
{
	if (list != NULL)
	{
		if (list->list_data != NULL)
			free(list->list_data);
		free(list);
	}
}

// 释放整个链表及其所有节点和数据
void ListDistoryAndFree(PLIST list)
{
	if (list != NULL)
	{
		ListClearAndFree(list);
		if (list->list_data != NULL)
			free(list->list_data);
		free(list);
	}
}

// 获取链表头的数据，并从链表中删除。
void* ListPopFront(PLIST list)
{
	PNODE tmp;
	void*  data = NULL;
	if (list != NULL)
	{
		if (list->first != NULL)
		{
			tmp = list->first;
			data = tmp->data;
			list->first = tmp->next;
			free(tmp);
		}
	}
	return (data);
}

// 获取链表尾的数据，并从链表中删除。
void* ListPopBack(PLIST list)
{
	PNODE prev;
	PNODE current;
	PNODE node = list->first;
	void* data;

	if (!node)
		return (NULL);
	if (!(node)->next)
	{
		data = (node)->data;
		free(node);
		node = NULL;
		return (data);
	}
	prev = node;
	current = (node)->next;
	while (current->next)
	{
		prev = current;
		current = current->next;
	}
	data = current->data;
	free(current);
	prev->next = NULL;
	return (data);
}

// 加入数据到链表尾部。
void ListPushBack(PLIST list, void* data)
{
	PNODE tmp;
	PNODE new_node = NodeCreate(data);

	if (new_node)
	{
		if (list)
		{
			if (list->first)
			{
				tmp = list->first;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = new_node;
			}
			else
				list->first = new_node;
		}
	}
}

// 加入数据到链表头部。
void ListPushFront(PLIST list, void* data)
{
	PNODE new_node;

	if (list != NULL)
	{
		new_node = NodeCreate(data);
		if (new_node != NULL)
		{
			new_node->next = list->first;
			list->first = new_node;
		}
	}
}


// 删除链表所有节点，free数据。
void ListClearAndFree(PLIST list)
{
	PNODE tmp;
	PNODE node;

	if (list != NULL)
	{
		node = list->first;
		while (node)
		{
			tmp = node->next;
			NodeDistory(node);
			node = tmp;
		}
		list->first = 0;
	}
}