echo "compileing all examples..."
g++ -o application  application.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] application compiled successfully"
g++ -o cookies_and_header_example  cookies_and_header_example.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] cookies_and_header_example compiled successfully"
g++ -o directory  directory.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] directory compiled successfully"
g++ -o dynamic_url  dynamic_url.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] dynamic_url compiled successfully"
g++ -o file_upload  file_upload.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] file_upload compiled successfully"
g++ -o middleware_example  middleware_example.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] middleware_example compiled successfully"
g++ -o post_form  post_form.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] post_form compiled successfully"
g++ -o redirect  redirect.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] redirect compiled successfully"
g++ -o session_example  session_example.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] session_example compiled successfully"
g++ -o simple  simple.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] simple compiled successfully"
g++ -o template  template.cpp -l ssl -l crypto -l pthread -l curl -std=c++20
echo "[+] template compiled successfully"