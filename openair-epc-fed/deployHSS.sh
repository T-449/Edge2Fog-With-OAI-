# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset


echo -e ${txtblu}Stopping HSS container${txtrst}
sudo docker stop prod-oai-hss
sudo docker container rm  prod-oai-hss


echo -e ${txtblu}Creating HSS container${txtrst}
python3 component/oai-hss/ci-scripts/generateConfigFiles.py --kind=HSS \
           --cassandra=192.168.62.2 \
           --hss_s6a=192.168.61.2 \
           --apn1=apn1.carrier.com --apn2=apn2.carrier.com \
           --users=200 --imsi=320230100000001 \
           --ltek=0c0a34601d4f07677303652c0462535b --op=63bfa50ee6523365ff14c1f45f88737d \
           --nb_mmes=1 \
           --from_docker_file --env_for_entrypoint
sudo docker create --privileged --name prod-oai-hss --network prod-oai-private-net --ip 192.168.62.3 --env-file ./hss-env.list oai-hss:production
sudo docker network connect --ip 192.168.61.2 prod-oai-public-net prod-oai-hss
sudo docker start prod-oai-hss
sleep 5
sudo docker attach prod-oai-hss #needed to attach in order to see output in terminal
