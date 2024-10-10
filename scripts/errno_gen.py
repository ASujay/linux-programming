import subprocess

if __name__ == "__main__":
    result = subprocess.run(['errno', '-l'], capture_output=True, text=True)
    result = result.stdout
    errorList = []
    for line in result.splitlines():
        errorList.append(line.split(' ')[0])

    with open('../lib/ename.h', 'w') as file:
        file.write("static char *ename[] = { \n")
        file.write("    /* 0*/ \"\"\n")
        for i in range(len(errorList)):
            if i % 7 == 0:
                file.write("    /* " + str(i + 1) + "*/ ")
            if i == len(errorList) - 1:
                file.write("\"" + errorList[i] + "\"\n")
            elif i % 7 == 6:
                file.write("\"" + errorList[i] + "\",\n")
            else:
                file.write("\"" + errorList[i] + "\", ")
        file.write("};\n");
        file.write("#define MAX_ENAME 134");
            
