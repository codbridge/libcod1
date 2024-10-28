bool Sys_IsProxyAddress(netadr_t from);
void SV_SetupProxies();
void SV_ShutdownProxies();
void * SV_StartProxy(void *threadArgs);
void * SV_ProxyMasterServerLoop(void *threadArgs);
void * SV_ProxyClientThread(void *threadArgs);