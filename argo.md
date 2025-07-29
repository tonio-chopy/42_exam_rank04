# Argo

## Parser
- peek a char
- parser string function
- parser integer
- parser map
- else com um unexpected

## parser string
- create the buffer
- expect the double quoute from stream
- get a letter
- verify EOF
- verify if is the closed double quoute (break)
- verify if is backslash
    - verify if is not EOF
- add in the buffer
- end add '\0' at the end of the buffer
- set value and type in DST
- strdup into the buffer for the string field in DST

## parser integer
- create the variable that will hold the value
- fscanf in the stream with verification = 1
- assign the values to the DST

## parser Map