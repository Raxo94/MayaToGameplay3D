WScript.Echo "Downloading using a fallback method. This might take a few minutes."
Dim strFileURL, strHDLocation
strFileURL = WScript.Arguments(0)
strHDLocation = WScript.Arguments(1)
Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
objXMLHTTP.open "GET", strFileURL, false
WScript.Echo "Sending request..."
objXMLHTTP.send()
If objXMLHTTP.Status = 200 Then
WScript.Echo "Got response. Processing body..."
Set objADOStream = CreateObject("ADODB.Stream")
objADOStream.Open
objADOStream.Type = 1
objADOStream.Write objXMLHTTP.ResponseBody
objADOStream.Position = 0
Set objFSO = Createobject("Scripting.FileSystemObject")
If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation
Set objFSO = Nothing
WScript.Echo "Saving result to a file..."
objADOStream.SaveToFile strHDLocation
objADOStream.Close
Set objADOStream = Nothing
WScript.Echo "Success."
End if
Set objXMLHTTP = Nothing
