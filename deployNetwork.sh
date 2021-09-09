# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset


echo -e ${txtblu}Pruning${txtrst}
sudo docker system prune 


# Creating prod-oai-public-net
echo -e ${txtblu}Creating bridged networks${txtrst}
sudo docker network create --attachable --subnet 192.168.61.0/26 --ip-range 192.168.61.0/26 prod-oai-public-net
sudo docker network create --attachable --subnet 192.168.62.0/26 --ip-range 192.168.62.0/26 prod-oai-private-net
