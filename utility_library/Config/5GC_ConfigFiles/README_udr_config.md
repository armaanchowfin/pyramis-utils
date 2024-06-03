# UDR Config File

**HOW TO ADD A SLICE INFORMATION IN udr_config.json**

To add a new supi range and nssai information:

- first update the ue-subscription in the ue-identities 
- update the num-ue-subscription-data-sets in the ue-subscription
- partition one the supi range and update that perticular array values
- insert a new array and give a correct key number 
- add the following in the new JSON object:
  - add the supi-set and insert the number added earlier
  - in nssai part add the following:
    - default-single-nssai and non-default-single-nssai like other ue-subscription-data
    - update the num-non-default-single-nssai
 
To add network slice information in the default-single-nssai and non-default-single-nssai:
- firstly add a single NSSAI (with sst and sd) then 
- add dnn-configurations (with 
  - dnn as dnnNo.sliceNo.net [No in sliceNo should be same as sst], 
  - default-dnn-indicator if the DNN is default DNN
  - pdu-session-types with default-session-type and allowed-session-type [currently allowed type is IPV4]
  - ssc-modes with default-ssc-mode and allowed-ssc-mode

