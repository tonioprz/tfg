from PyPDF2 import PdfFileReader
from pdf2jpg import pdf2jpg

inputpath = "CAJA1.pdf"
outputpath = "output"
# To convert single page
result = pdf2jpg.convert_pdf2jpg(inputpath, outputpath, dpi=400, pages="2")
print(result)
