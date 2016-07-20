# -*- coding: utf-8 -*-
import re
import string

def main():
	total = []
	files = [open("trust.log"),open("nontrust.log")]
	# 1. create a list of [time,string]
	for i, fd in enumerate(files):
		for line in fd:
			match = re.match('\[(\d*)\](.*)', line)
			if match:
				groups = list(match.groups())
				groups.append(str(i+1))
				total.append(groups)
	# 2. sort the list by time
	s = sorted(total, key=lambda value: value[0], reverse=False)
	# 3. insert "[2] leave from TRUST" after "[1] dispatch to LTASK"
	TRUST_ID = "13"
	LTASK_RANGE = range(6,9)
	NT_RANGE = range(9,13)
	nt_running = 9
	total = []
	for entry in s:
		total.append(entry)
		match = re.match(' dispatch to task (\d*).', entry[1])
		if match:
			task = int(match.groups()[0])
			if task in LTASK_RANGE:
				total.append([entry[0], " dispatch from task "+TRUST_ID+".", "2"])
				total.append([entry[0], " dispatch to task "+str(nt_running)+".", "2"])
			elif task in NT_RANGE:
				nt_running = task
	# 4. detect NT -> T edges and insert a "[2] enter to TRUST"
	nt_running = 9
	prev_proc_id = 1
	s = total
	total = []
	for entry in s:
		match = re.match(' dispatch to task (\d*).', entry[1])
		if match:
			task = int(match.groups()[0])
			if task in NT_RANGE:
				nt_running = task
		if (int(entry[2]) == 1) and (prev_proc_id == 2): # NT -> T
			total.append([entry[0], " dispatch from task "+str(nt_running)+".", "2"])
			total.append([entry[0], " dispatch to task "+TRUST_ID+".", "2"])
		prev_proc_id = int(entry[2])
		total.append(entry)
	# 5. fix dispatches that were interrupted by TRUST
	for i, entry in enumerate(total):
		if entry[2] == "1":
			continue
		match = re.match(' dispatch to task (\d*).', entry[1])
		if match:
			match = re.match(' dispatch to task (\d*).', total[i-1][1])
			if match:
				del total[i-1]
		else:
			match = re.match(' dispatch from task (\d*).', entry[1])
			if match:
				match = re.match(' dispatch from task (\d*).', total[i-1][1])
				if match:
					del total[i-1]
	# 5. write to a combined log
	out = open("combined.log","w")
	for entry in total:
		out.write("["+entry[0]+"]:["+entry[2]+"]:"+entry[1]+"\n")

if __name__ == '__main__':
	main()




