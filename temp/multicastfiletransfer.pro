TEMPLATE = subdirs

SUBDIRS += \
    multicastfiletransferserver \
    multicastfiletransferclient
    #Multicast Group : 239.255.0.1
    #Port            : 45454
    #ACK Port        : 45455
    #CHUNK_SIZE      : 1400 - header
    #WINDOW_SIZE     : 64
