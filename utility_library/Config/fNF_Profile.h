#ifndef NRF_MSG_H
#define NRF_MSG_H


#include "../Logging/include/Log.h"


#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP_NOCONTENT 204
#define HTTP_BADREQUEST 400
#define HTTP_FORBIDDEN 403
#define HTTP_NOTFOUND 404
#define LENGTHREQUIRED 411
#define PRECONDITIONFAILED 413

enum NFType {NRF, UDM, AMF, SMF, AUSF, NEF, PCF, SMSF, NSSF, UDR, LMF, GMLC, G_EIR, SEPP, UPF, N3IWF, AF, UDSF, BSF, CHF,NFNOTFOUND} ;

enum NFStatus {REGISTERED, SUSPENDED};

enum DataSetId {SUBSCRIPTION, POLICY, EXPOSURE, APPLICATION };

enum UPInterfaceType { N3,N6,N9};

enum UriScheme{http,https}; 


using namespace std;

struct PlmnId {
    string mcc;     

    string mnc;     

};

struct Snssai{
    uint8_t sst;        

    string sd;          

};

struct SupiRange{
    string start;
    string end;
    string pattern;
};

struct IdentityRange{
    string start;
    string end;
    string pattern;
};

struct Guami{
    PlmnId plmnId;           

    string amfId;            
 

};


struct Tai{
  PlmnId plmnId;    

  string Tac;       
 

};

struct TacRange{
    string start;   

    string end;     

    string pattern;
};

struct TaiRange{
    PlmnId plmnId;  

    vector<TacRange> tacRangeList;
};

struct  BackupInfoAmfRemoval{
    PlmnId plmnId;
    typedef string AmfId ;
};

 struct N2InterfaceAmfInfo{
    vector<string> ipv4EndpointAddress;    

    vector<string> ipv6EndpointAddress;    

    string amfName;
};

struct DnnUpfInfoItem{
    string dnn;
};

struct SnssaiUpfInfoItem{
  Snssai nssai;                            

  vector <DnnUpfInfoItem> dnnUpfInfoList; 


};

struct InterfaceUpfInfoItem{
  string interfaceType; 
  vector<string> ipv4EndpointAddress;
  vector<string> ipv6EndpointAddress;
  string endpointFqdn;
  string networkInstance;
};

struct Ipv4AddressRange{
    string start;
    string end;
};

struct Ipv6PrefixRange{
    string start;
    string end;
};

struct UdrInfo{
    string groupId;
    vector<SupiRange> supiRanges;
    vector<IdentityRange> gpsiRanges;
    vector<IdentityRange> externalGroupIdentityfiersRanges;
    vector<string> supportDataSets;
};

struct UdmInfo {
    string groupId;
    vector<SupiRange> supiRanges;
    vector<IdentityRange> gpsiRanges;
        vector<IdentityRange> externalGroupIdentityfiersRanges;
    vector<string> routingIndicators; 
};

struct Ausfinfo {
    string groupId;
    vector<SupiRange> supiRanges;
    vector<string> routingIndicators;
};

struct AmfInfo{
    string amfSetId;                

    string amfRegionId;             

    vector<Guami>  guamiList;       
   
    vector<Tai> taiList;
    vector<TaiRange> taiRangeList;
    vector<Guami> backupInfoAmfFailure;
    vector<Guami> backupInfoAmfRemoval;
    N2InterfaceAmfInfo n2InterfaceAmfInfo;
 };

struct DnnSmfInfoItem {
    string dnn;                                 

};

struct SnssaiSmfInfoItem {
    Snssai sNssai;                              

    vector<DnnSmfInfoItem> dnnSmfInfoList;      

};

struct SmfInfo
{
    vector<SnssaiSmfInfoItem> sNssaiSmfInfoList;    

    vector<string> dnnList;
    vector<Tai> taiList;
    vector<TaiRange> taiRangeList;
    string pgwFqdn;
    vector<string> accessType;
};

struct UpfInfo{
  vector<SnssaiUpfInfoItem> sNssaiUpfInfoList; 

  vector<string> smfServingArea;
  vector<InterfaceUpfInfoItem> interfaceUpfInfoList;  
};

struct PcfInfo{
    vector<string> dnnList;
    vector<SupiRange> supiRangeList;
};

struct BsfInfo{
    vector<Ipv4AddressRange> ipv4AddressRanges;
    vector<Ipv6PrefixRange> ipv6PrefixRanges;
};


struct NFServiceVersion{
    string apiVersionInUri;     
    

    string apiFullVersion;      
    

    string expiry;                              

};
 
struct IpEndPoint{
    string ipv4Address;
    string ipv6Address;
    string transport;       

    int port;
};


struct DefaultNotificationSubscription{
    string notificationType;    
 

    string callbackuri;         

    string n1MessageClass;                   

    string n2InformatonClass;                

};


struct NFService{
    
    string serviceInstanceId;   

    string serviceName;         
 

    vector<NFServiceVersion> versions;  

    string scheme;              
    

    string nfServiceStatus;     
    

    string fqdn;
    vector<IpEndPoint> ipEndPoints;
    string apiPrefix;
    vector<DefaultNotificationSubscription> defaultNotificationSubscriptions;
    int capacity;       

    int load;           

    int priority;       

    string recoveryTime;        

    string supportedFeatures;   

    
    
    string interPlmnFqdn;
    PlmnId allowedPlmns;
    Snssai allowedNsssais;
 };


struct NFProfile{
    string nfInstanceId;        
  

    string nfType;              
  

    string nfStatus;            
  

    PlmnId plmn;                

    vector<Snssai> sNssais;
    vector<string> nsiList;
    string fqdn;
    string interPlmnFqdn;
    vector<string> ipv4Addresses;     
 
    vector<string> ipv6Addresses;

    int priority;           

    int capacity;           

    int load;               

    string locality;
    
    UdrInfo udrInfo;
    UdmInfo udmInfo;
    Ausfinfo ausfInfo;
    AmfInfo amfInfo;
    SmfInfo smfInfo;
    UpfInfo upfInfo;
    PcfInfo pcfInfo;
    BsfInfo bsfInfo;
     

    string recoveryTime;    

    
    
    
    vector<NFService> nfServices;
    
    
    string ipv6Prefixes;
};


struct NFRegistrationData{
    
    int heartBeatTimer ;
    
    NFProfile nfProfile;

    NFRegistrationData() {
        heartBeatTimer = 0; 

        nfProfile = {};
    }
};

#endif
