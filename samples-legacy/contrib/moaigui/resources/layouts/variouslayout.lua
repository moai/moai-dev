
local data = {
	label1 = {
		widget = "label",
		pos = {5, 3},
		dim = {50, 5},
		text = "Just some simple text",
	},
	button1 = {
		widget = "button",
		pos = {5, 10},
		dim = {30, 10},
		text = "button 1",
	},
	button2 = {
		widget = "button",
		pos = {45, 10},
		dim = {30, 20},
		text = "button 2",
		images = {
			normal = {
				{
					fileName = "moai.png",
					color = {1, 1, 1, 1},
				},
			},
			hover = {
				{
					fileName = "moai.png",
					color = {1, 0, 0, 1},
				},
			},
			pushed = {
				{
					fileName = "moai.png",
					color = {0, 1, 0, 1},
				},
			},
			disabled = {
				{
					fileName = "moai.png",
					color = {0, 0, 0, 1},
				},
			},
		},
	},
	image1 = {
		widget = "image",
		pos = {5, 23},
		dim = {22, 15},
		text = "Image",
		textHorzAlign = "center",
		textVertAlign = "bottom",
		images = {
			{
				fileName = "moai.png",
			},
		},
	},
	-- progressBar1 = {
		-- widget = "progress bar",
		-- pos = {45, 33},
		-- dim = {30, 5},
	-- },
	label2 = {
		widget = "label",
		pos = {5, 60},
		dim = {30, 5},
		text = "Slide Me",
	},
	slider1 = {
		widget = "slider",
		pos = {35, 60},
		dim = {50, 5},
		vert = false,
		maxValue = 20,
		minValue = 1,
	},
	label3 = {
		widget = "label",
		pos = {5, 70},
		dim = {30, 5},
		text = "Enter Text",
	},
	edit1 = {
		widget = "edit box",
		pos = {35, 70},
		dim = {50, 5},
	},
}

for i = 1, 3 do
	local radio = {}
	radio.widget = "radio button"
	radio.pos = {(i - 1) * 25 + 5, 40}
	radio.dim = {20, 4}
	radio.text = "Radio"
	radio.radioButtonGroup = 1

	data["radio" .. i] = radio
end

for i = 1, 3 do
	local check = {}
	check.widget = "check box"
	check.pos = {(i - 1) * 25 + 5, 50}
	check.dim = {20, 4}
	check.text = "Check"
	check.radioButtonGroup = 1

	data["check" .. i] = check
end

return data
