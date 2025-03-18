# Define all the functions used to setup the latex output format for the cutflow files

def latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline") # Number of Weighted Entries
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline \\hline")

def jetSplit_event_latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" &  & All & Valid Jet \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" &  & All & Valid Jet \\\\ \\hline \\hline")

def event_latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|ll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\textbf{Fraction of Entries} \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|ll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\textbf{Fraction of Entries} \\\\ \\hline \\hline")

def latex_end(file_path):
    print("\\hline")
    print("\\end{tabular}")
    print("\\caption{\\protect\\url{" + file_path + "}}") # to save which file this cutflow is from
    print("\\label{9-Table:BkgEst_Tower}")
    print("\\end{table}")