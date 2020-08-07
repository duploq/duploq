DUPLO_VERSION=0.5.0

# console & studio build
msvc*{
    DEFINES += DUPLO_VERSION=\"$${DUPLO_VERSION}\"
}

# gcc
gcc{
    DEFINES += DUPLO_VERSION=\"\\\"$${DUPLO_VERSION}\"\\\"
}

# creator build
#DEFINES += DUPLO_VERSION=\"\\\"$${DUPLO_VERSION}\"\\\"
