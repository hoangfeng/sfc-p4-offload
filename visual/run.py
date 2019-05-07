import webbrowser

json= '{\"nodes\":['
f = open("../function.txt", "r")
count = 0
for line in f:
	count= count +1
	element = line.split();
	element = element[1:]
	temp= "\"label\":\"Node " + str(count) + " ["
	for e in element:
		temp+= e + ','
	print element
	if len(element) > 0:
		temp = temp[:-1]
	temp+= "]\""
	json += "{\"id\":"+str(count)+","+temp+"},"
json= json[:-1]
json+="],\"edges\":["

f = open("../output.txt", "r")
count = 0
for line in f:
	count= count +1
	element = line.split();
	element = element[1:]
	temp= "{\"from\":" + element[0]+ ",\"to\":" + element[1] +",\"arrows\":\"to\",\"label\":\""+element[2]+"\"}"
	json += temp+","
json= json[:-1]
json+="]}"


file = open('node.json', 'w')
file.write(json)
file.close()
print(json)
webbrowser.open("index.html");
