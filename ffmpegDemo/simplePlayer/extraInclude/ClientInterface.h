#ifndef __CLIENT_INTERFACE_H__
#define __CLIENT_INTERFACE_H__

extern "C"{

	/*
		����:����client��
		����:
			ipBuf Զ��IP��ַ
			port  Զ�˶˿�
			streamName ����
		����ֵ:
			0 ʧ��
			1 �ɹ�
	*/
	int startClient(char *ipBuf,unsigned short port,char *streamName);
	/*
		����: ֹͣ�ͻ���
		����:
			��
		����ֵ:
			0 ʧ��
			1 �ɹ�
	*/
	int stopClient();

	/*
		��ȡ����
		����:
			frameBuf ������ָ�룬������ֻ��Ҫ����ָ�뼴��
			frameLen ֡����
		����ֵ: 
			 1 ��ȡ�ɹ�
			-1 Client�����˳�,�����߿ɸ��ݸ�ֵ�жϿͻ����Ƿ��쳣�˳�
			-2 ���� frameLen Ϊ�� 
	*/
	int readData(unsigned char **frameBuf,unsigned int *frameLen);
}

#endif
