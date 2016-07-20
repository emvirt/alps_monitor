{
	"TimeScale" :"us",
	"TimeRadix" :10,
	"ConvertRules"   :["fmp"],
	"VisualizeRules" :["toppers","fmp","fmp_core2"],
	"ResourceHeaders":["fmp"],
	"Resources":
	{
		"CurrentContext_PRC1":{
			"Type":"Context",
			"Attributes":
			{
				"name"    : "None"
			}
		},
		"CurrentContext_PRC2":{
			"Type":"Context",
			"Attributes":
			{
				"name"    : "None"
			}
		},
		"MAIN_TASK":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :1,
				"atr"   :"TA_ACT",
				"pri"   :4,
				"exinf" :"0",
				"task"  :"main_task",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK1":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :2,
				"atr"   :"TA_NULL",
				"pri"   :4,
				"exinf" :"1",
				"task"  :"task1",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK2":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :3,
				"atr"   :"TA_NULL",
				"pri"   :8,
				"exinf" :"2",
				"task"  :"task2",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK3":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :4,
				"atr"   :"TA_NULL",
				"pri"   :10,
				"exinf" :"3",
				"task"  :"task3",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"LMANG":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :5,
				"atr"   :"TA_ACT",
				"pri"   :6,
				"exinf" :"0",
				"task"  :"latency_manager",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"LTASK0":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :6,
				"atr"   :"TA_ACT",
				"pri"   :7,
				"exinf" :"0",
				"task"  :"ltask",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"LTASK1":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :7,
				"atr"   :"TA_ACT",
				"pri"   :9,
				"exinf" :"1",
				"task"  :"ltask",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"BTASK":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :1,
				"id"    :8,
				"atr"   :"TA_ACT",
				"pri"   :11,
				"exinf" :"2",
				"task"  :"ltask",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"NT_MAIN_TASK":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :2,
				"id"    :9,
				"atr"   :"TA_ACT",
				"pri"   :4,
				"exinf" :"0",
				"task"  :"main_task",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"NT_TASK1":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :2,
				"id"    :10,
				"atr"   :"TA_NULL",
				"pri"   :5,
				"exinf" :"1",
				"task"  :"task1",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"NT_TASK2":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :2,
				"id"    :11,
				"atr"   :"TA_NULL",
				"pri"   :8,
				"exinf" :"2",
				"task"  :"task2",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"NT_TASK3":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :2,
				"id"    :12,
				"atr"   :"TA_ACT",
				"pri"   :12,
				"exinf" :"3",
				"task"  :"task3",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TRUST":{
			"Type":"Task",
			"Attributes":
			{
				"prcId" :2,
				"id"    :13,
				"atr"   :"TA_ACT",
				"pri"   :12,
				"exinf" :"3",
				"task"  :"task3",
				"stksz" :4096,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"INH_INHNO_TIMER_target_timer_handler":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :1,
				"id"    :9,
				"state"    : "DORMANT"
			}
		},
		"INH_NT_INTNO_TIMER_latency_handler_timer":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :1,
				"id"    :8,
				"state"    : "DORMANT"
			}
		},
		"INH_NT_INTNO_SOFTIRQ_latency_handler_softirq":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :1,
				"id"    :1,
				"state"    : "DORMANT"
			}
		},
		"INH_19__kernel_inthdr_19":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :1,
				"id"    :19,
				"state"    : "DORMANT"
			}
		},
		"INH_20__kernel_inthdr_20":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :1,
				"id"    :20,
				"state"    : "DORMANT"
			}
		},
		"INH_INHNO_TIMER_nt_target_timer_handler":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :2,
				"id"    :40,
				"state"    : "DORMANT"
			}
		},
		"INH_50__kernel_inthdr_50":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"prcIdI" :2,
				"id"    :50,
				"state"    : "DORMANT"
			}
		},
		"CYC1":{
			"Type":"CyclicHandler",
			"Attributes":
			{
				"prcIdC" :1,
				"id"    :1,
				"state"    : "DORMANT"
			}
		},
		"CYC_LTASK0":{
			"Type":"CyclicHandler",
			"Attributes":
			{
				"prcIdC" :1,
				"id"    :2,
				"state"    : "DORMANT"
			}
		},
		"CYC_LTASK1":{
			"Type":"CyclicHandler",
			"Attributes":
			{
				"prcIdC" :1,
				"id"    :3,
				"state"    : "DORMANT"
			}
		},
		"NT_CYC":{
			"Type":"CyclicHandler",
			"Attributes":
			{
				"prcIdC" :2,
				"id"    :4,
				"state"    : "DORMANT"
			}
		},
		"TASK_TEX":{
			"Type":"TaskExceptionRoutine",
			"Attributes":
			{
				"prcIdX" :1,
				"state"    : "DORMANT"
			}
		}
	}
}

