#include <stdlib.h>
#include <pthread.h>
#include "locker_pthread.h"

typedef struct _PrivInfo               //定意私有数据
{
	pthread_mutex_t mutex;           //互斥量
}PrivInfo;

static Ret  locker_pthread_lock(Locker* thiz)    //加锁函数
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_lock(&priv->mutex);

	return ret == 0 ? RET_OK : RET_FAIL;
}

static Ret  locker_pthread_unlock(Locker* thiz)   //解锁函数
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_unlock(&priv->mutex);

	return ret == 0 ? RET_OK : RET_FAIL;
}

static void locker_pthread_destroy(Locker* thiz)  //销毁函数
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_destroy(&priv->mutex);

	SAFE_FREE(thiz);

	return;
}

Locker* locker_pthread_create(void)               //创建函数
{
	Locker* thiz = (Locker*)malloc(sizeof(Locker) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->lock    = locker_pthread_lock;
		thiz->unlock  = locker_pthread_unlock;
		thiz->destroy = locker_pthread_destroy;

		pthread_mutex_init(&(priv->mutex), NULL);
	}

	return thiz;
}
