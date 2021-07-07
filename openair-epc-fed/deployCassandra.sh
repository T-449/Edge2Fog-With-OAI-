# colors
txtblk='\033[0;30m' # Black - Regular
txtred='\033[0;31m' # Red
txtgrn='\033[0;32m' # Green
txtblu='\033[0;34m' # Blue
txtpur='\033[0;35m' # Purple
txtwht='\033[0;37m' # White
txtrst='\033[0m'    # Text Reset


echo -e ${txtblu}Stopping Cassandra container${txtrst}
sudo docker stop prod-cassandra
sudo docker container rm  prod-cassandra



echo -e ${txtblu}Creating Cassandra container${txtrst}
sudo docker run --name prod-cassandra --network prod-oai-private-net --ip 192.168.62.2 -d \
             -e CASSANDRA_CLUSTER_NAME="OAI HSS Cluster" \
             -e CASSANDRA_ENDPOINT_SNITCH=GossipingPropertyFileSnitch cassandra:2.1
sudo docker cp component/oai-hss/src/hss_rel14/db/oai_db.cql prod-cassandra:/home
echo Sleeping
sleep 5
sudo docker exec -it prod-cassandra /bin/bash -c "nodetool status"
echo Sleeping
sleep 5
sudo docker exec -it prod-cassandra /bin/bash -c "cqlsh --file /home/oai_db.cql 192.168.62.2"
