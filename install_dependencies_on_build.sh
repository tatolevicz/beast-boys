
#this path like this because this script will be called from cmake build so it need to step back to put de dependencies on project
DIR="../external"
if [ -d "$DIR/rapidjson" ]; then
  echo "$DIR rapidjson exists!"
  else
  echo "$DIR Downloading rapidjson:"
  git clone https://github.com/Tencent/rapidjson.git "$DIR/rapidjson"
fi
