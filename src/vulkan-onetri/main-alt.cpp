#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <set>
#include <glm/glm.hpp>
#include <assert.h>

// Global Settings
const char						gAppName[] = "VulkanDemo";
const char						gEngineName[] = "VulkanDemoEngine";
int								gWindowWidth = 1280;
int								gWindowHeight = 720;
VkPresentModeKHR				gPresentationMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
VkSurfaceTransformFlagBitsKHR	gTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
VkFormat						gFormat = VK_FORMAT_B8G8R8A8_SRGB;
VkColorSpaceKHR					gColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
VkImageUsageFlags				gImageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

/**
 * This demo attempts to create a window and vulkan compatible surface using SDL
 * Verified and tested using multiple CPUs under windows.
 * Should work on every other SDL / Vulkan supported operating system (OSX, Linux, Android)
 * main() clearly outlines all the specific steps taken to create a vulkan instance,
 * select a device, create a vulkan compatible surface (opaque) associated with a window.
 */

//////////////////////////////////////////////////////////////////////////
// Global Settings
//////////////////////////////////////////////////////////////////////////

/**
 *	@return the set of layers to be initialized with Vulkan
 */
const std::set<std::string>& getRequestedLayerNames()
{
	static std::set<std::string> layers;
	if (layers.empty())
	{
		layers.emplace("VK_LAYER_NV_optimus");
		layers.emplace("VK_LAYER_LUNARG_standard_validation");
	}
	return layers;
}


/**
 * @return the set of required device extension names
 */
const std::set<std::string>& getRequestedDeviceExtensionNames()
{
	static std::set<std::string> layers;
	if (layers.empty())
	{
		layers.emplace(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}
	return layers;
}


/**
 * @return the set of required image usage scenarios
 * that need to be supported by the surface and swap chain
 */
const std::vector<VkImageUsageFlags> getRequestedImageUsages()
{
	static std::vector<VkImageUsageFlags> usages;
	if (usages.empty())
	{
		usages.emplace_back(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
	}
	return usages;
}


//////////////////////////////////////////////////////////////////////////
// Utilities
//////////////////////////////////////////////////////////////////////////

/**
 * Clamps value between min and max
 */
template<typename T>
T clamp(T value, T min, T max)
{
	return glm::clamp<T>(value, min, max);
}


//////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////

/**
* Initializes SDL
* @return true if SDL was initialized successfully
*/
bool initSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
		return true;
	std::cout << "Unable to initialize SDL\n";
	return false;
}


/**
 * Callback that receives a debug message from Vulkan
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData)
{
	std::cout << "validation layer: " << layerPrefix << ": " << msg << std::endl;
	return VK_FALSE;
}


VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


/**
 *	Sets up the vulkan messaging callback specified above
 */
bool setupDebugCallback(VkInstance instance, VkDebugReportCallbackEXT& callback)
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	if (createDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
	{
		std::cout << "unable to create debug report callback extension\n";
		return false;
	}
	return true;
}


/**
 * Destroys the callback extension object
 */
void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}


bool getAvailableVulkanLayers(std::vector<std::string>& outLayers)
{
	// Figure out the amount of available layers
	// Layers are used for debugging / validation etc / profiling..
	unsigned int instance_layer_count = 0;
	VkResult res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
	if (res != VK_SUCCESS)
	{
		std::cout << "unable to query vulkan instance layer property count\n";
		return false;
	}

	std::vector<VkLayerProperties> instance_layer_names(instance_layer_count);
	res = vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layer_names.data());
	if (res != VK_SUCCESS)
	{
		std::cout << "unable to retrieve vulkan instance layer names\n";
		return false;
	}

	// Display layer names and find the ones we specified above
	std::cout << "found " << instance_layer_count << " instance layers:\n";
	std::vector<const char*> valid_instance_layer_names;
	const std::set<std::string>& lookup_layers = getRequestedLayerNames();
	int count(0);
	outLayers.clear();
	for (const auto& name : instance_layer_names)
	{
		std::cout << count << ": " << name.layerName << ": " << name.description << "\n";
		auto it = lookup_layers.find(std::string(name.layerName));
		if (it != lookup_layers.end())
			outLayers.emplace_back(name.layerName);
		count++;
	}

	// Print the ones we're enabling
	std::cout << "\n";
	for (const auto& layer : outLayers)
		std::cout << "applying layer: " << layer.c_str() << "\n";
	return true;
}


bool getAvailableVulkanExtensions(SDL_Window* window, std::vector<std::string>& outExtensions)
{
	// Figure out the amount of extensions vulkan needs to interface with the os windowing system
	// This is necessary because vulkan is a platform agnostic API and needs to know how to interface with the windowing system
	unsigned int ext_count = 0;
	if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, nullptr))
	{
		std::cout << "Unable to query the number of Vulkan instance extensions\n";
		return false;
	}

	// Use the amount of extensions queried before to retrieve the names of the extensions
	std::vector<const char*> ext_names(ext_count);
	if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, ext_names.data()))
	{
		std::cout << "Unable to query the number of Vulkan instance extension names\n";
		return false;
	}

	// Display names
	std::cout << "found " << ext_count << " Vulkan instance extensions:\n";
	for (unsigned int i = 0; i < ext_count; i++)
	{
		std::cout << i << ": " << ext_names[i] << "\n";
		outExtensions.emplace_back(ext_names[i]);
	}

	// Add debug display extension, we need this to relay debug messages
	outExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	std::cout << "\n";
	return true;
}


/**
 * Creates a vulkan instance using all the available instance extensions and layers
 * @return if the instance was created successfully
 */
bool createVulkanInstance(const std::vector<std::string>& layerNames, const std::vector<std::string>& extensionNames, VkInstance& outInstance)
{
	// Copy layers
	std::vector<const char*> layer_names;
	for (const auto& layer : layerNames)
		layer_names.emplace_back(layer.c_str());

	// Copy extensions
	std::vector<const char*> ext_names;
	for (const auto& ext : extensionNames)
		ext_names.emplace_back(ext.c_str());

	// Get the suppoerted vulkan instance version
	unsigned int api_version;
	vkEnumerateInstanceVersion(&api_version);

	// initialize the VkApplicationInfo structure
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = gAppName;
	app_info.applicationVersion = 1;
	app_info.pEngineName = gEngineName;
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure
	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = static_cast<uint32_t>(ext_names.size());
	inst_info.ppEnabledExtensionNames = ext_names.data();
	inst_info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
	inst_info.ppEnabledLayerNames = layer_names.data();

	// Create vulkan runtime instance
	std::cout << "initializing Vulkan instance\n\n";
	VkResult res = vkCreateInstance(&inst_info, NULL, &outInstance);
	switch (res)
	{
	case VK_SUCCESS:
		break;
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		std::cout << "unable to create vulkan instance, cannot find a compatible Vulkan ICD\n";
		return false;
	default:
		std::cout << "unable to create Vulkan instance: unknown error\n";
		return false;
	}
	return true;
}


/**
 * Allows the user to select a GPU (physical device)
 * @return if query, selection and assignment was successful
 * @param outDevice the selected physical device (gpu)
 * @param outQueueFamilyIndex queue command family that can handle graphics commands
 */
bool selectGPU(VkInstance instance, VkPhysicalDevice& outDevice, unsigned int& outQueueFamilyIndex)
{
	// Get number of available physical devices, needs to be at least 1
	unsigned int physical_device_count(0);
	vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
	if (physical_device_count == 0)
	{
		std::cout << "No physical devices found\n";
		return false;
	}

	// Now get the devices
	std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
	vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

	// Show device information
	std::cout << "found " << physical_device_count << " GPU(s):\n";
	int count(0);
	std::vector<VkPhysicalDeviceProperties> physical_device_properties(physical_devices.size());
	for (auto& physical_device : physical_devices)
	{
		vkGetPhysicalDeviceProperties(physical_device, &(physical_device_properties[count]));
		std::cout << count << ": " << physical_device_properties[count].deviceName << "\n";
		count++;
	}

	// Select one if more than 1 is available
	unsigned int selection_id = 0;
	if (physical_device_count > 1)
	{
		while (true)
		{
			std::cout << "select device: ";
			std::cin  >> selection_id;
			if (selection_id >= physical_device_count || selection_id < 0)
			{
				std::cout << "invalid selection, expected a value between 0 and " << physical_device_count - 1 << "\n";
				continue;
			}
			break;
		}
	}
	std::cout << "selected: " << physical_device_properties[selection_id].deviceName << "\n";
	VkPhysicalDevice selected_device = physical_devices[selection_id];

	// Find the number queues this device supports, we want to make sure that we have a queue that supports graphics commands
	unsigned int family_queue_count(0);
	vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, nullptr);
	if (family_queue_count == 0)
	{
		std::cout << "device has no family of queues associated with it\n";
		return false;
	}
	
	// Extract the properties of all the queue families
	std::vector<VkQueueFamilyProperties> queue_properties(family_queue_count);
	vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, queue_properties.data());

	// Make sure the family of commands contains an option to issue graphical commands.
	unsigned int queue_node_index = -1;
	for (unsigned int i = 0; i < family_queue_count; i++)
	{
		if (queue_properties[i].queueCount > 0 && queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queue_node_index = i;
			break;
		}
	}

	if (queue_node_index < 0)
	{
		std::cout << "Unable to find a queue command family that accepts graphics commands\n";
		return false;
	}

	// Set the output variables
	outDevice = selected_device;
	outQueueFamilyIndex = queue_node_index;
	return true;
}


/**
 *	Creates a logical device
 */
bool createLogicalDevice(VkPhysicalDevice& physicalDevice,
	unsigned int queueFamilyIndex,
	const std::vector<std::string>& layerNames,
	VkDevice& outDevice)
{
	// Copy layer names
	std::vector<const char*> layer_names;
	for (const auto& layer : layerNames)
		layer_names.emplace_back(layer.c_str());


	// Get the number of available extensions for our graphics card
	uint32_t device_property_count(0);
	if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, NULL) != VK_SUCCESS)
	{
		std::cout << "Unable to acquire device extension property count\n";
		return false;
	}
	std::cout << "\nfound " << device_property_count << " device extensions\n";

	// Acquire their actual names
	std::vector<VkExtensionProperties> device_properties(device_property_count);
	if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, device_properties.data()) != VK_SUCCESS)
	{
		std::cout << "Unable to acquire device extension property names\n";
		return false;
	}

	// Match names against requested extension
	std::vector<const char*> device_property_names;
	const std::set<std::string>& required_extension_names = getRequestedDeviceExtensionNames();
	int count = 0;
	for (const auto& ext_property : device_properties)
	{
		std::cout << count << ": " << ext_property.extensionName << "\n";
		auto it = required_extension_names.find(std::string(ext_property.extensionName));
		if (it != required_extension_names.end())
		{
			device_property_names.emplace_back(ext_property.extensionName);
		}
		count++;
	}

	// Warn if not all required extensions were found
	if (required_extension_names.size() != device_property_names.size())
	{
		std::cout << "not all required device extensions are supported!\n";
		return false;
	}

	std::cout << "\n";
	for (const auto& name : device_property_names)
		std::cout << "applying device extension: " << name << "\n";

	// Create queue information structure used by device based on the previously fetched queue information from the physical device
	// We create one command processing queue for graphics
	VkDeviceQueueCreateInfo queue_create_info;
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = queueFamilyIndex;
	queue_create_info.queueCount = 1;
	std::vector<float> queue_prio = { 1.0f };
	queue_create_info.pQueuePriorities = queue_prio.data();
	queue_create_info.pNext = NULL;
	queue_create_info.flags = NULL;

	// Device creation information
	VkDeviceCreateInfo create_info;
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.queueCreateInfoCount = 1;
	create_info.pQueueCreateInfos = &queue_create_info;
	create_info.ppEnabledLayerNames = layer_names.data();
	create_info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
	create_info.ppEnabledExtensionNames = device_property_names.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(device_property_names.size());
	create_info.pNext = NULL;
	create_info.pEnabledFeatures = NULL;
	create_info.flags = NULL;

	// Finally we're ready to create a new device
	VkResult res = vkCreateDevice(physicalDevice, &create_info, nullptr, &outDevice);
	if (res != VK_SUCCESS)
	{
		std::cout << "failed to create logical device!\n";
		return false;
	}
	return true;
}


/**
 *	Returns the vulkan device queue associtated with the previously created device
 */
void getDeviceQueue(VkDevice device, int familyQueueIndex, VkQueue& outGraphicsQueue)
{
	vkGetDeviceQueue(device, familyQueueIndex, 0, &outGraphicsQueue);
}


/**
 *	Creates the vulkan surface that is rendered to by the device using SDL
 */
bool createSurface(SDL_Window* window, VkInstance instance, VkPhysicalDevice gpu, uint32_t graphicsFamilyQueueIndex, VkSurfaceKHR& outSurface)
{
	if (!SDL_Vulkan_CreateSurface(window, instance, &outSurface))
	{
		std::cout << "Unable to create Vulkan compatible surface using SDL\n";
		return false;
	}

	// Make sure the surface is compatible with the queue family and gpu
	VkBool32 supported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, graphicsFamilyQueueIndex, outSurface, &supported);
	if (!supported)
	{
		std::cout << "Surface is not supported by physical device!\n";
		return false;
	}

	return true;
}


/**
 * @return if the present modes could be queried and ioMode is set
 * @param outMode the mode that is requested, will contain FIFO when requested mode is not available
 */
bool getPresentationMode(VkSurfaceKHR surface, VkPhysicalDevice device, VkPresentModeKHR& ioMode)
{
	uint32_t mode_count(0);
	if(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, NULL) != VK_SUCCESS)
	{
		std::cout << "unable to query present mode count for physical device\n";
		return false;
	}

	std::vector<VkPresentModeKHR> available_modes(mode_count);
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, available_modes.data()) != VK_SUCCESS)
	{
		std::cout << "unable to query the various present modes for physical device\n";
		return false;
	}

	for (auto& mode : available_modes)
	{
		if (mode == ioMode)
			return true;
	}
	std::cout << "unable to obtain preferred display mode, fallback to FIFO\n";
	ioMode = VK_PRESENT_MODE_FIFO_KHR;
	return true;
}


/**
 * Obtain the surface properties that are required for the creation of the swap chain
 */
bool getSurfaceProperties(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities)
{
	if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities) != VK_SUCCESS)
	{
		std::cout << "unable to acquire surface capabilities\n";
		return false;
	}
	return true;
}


/**
 * Figure out the number of images that are used by the swapchain and
 * available to us in the application, based on the minimum amount of necessary images
 * provided by the capabilities struct.
 */
unsigned int getNumberOfSwapImages(const VkSurfaceCapabilitiesKHR& capabilities)
{
	unsigned int number = capabilities.minImageCount + 1;
	return number > capabilities.maxImageCount ? capabilities.minImageCount : number;
}


/**
 *	Returns the size of a swapchain image based on the current surface
 */
VkExtent2D getSwapImageSize(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// Default size = window size
	VkExtent2D size = { (unsigned int)gWindowWidth, (unsigned int)gWindowHeight };
	
	// This happens when the window scales based on the size of an image
	if (capabilities.currentExtent.width == 0xFFFFFFF)
	{
		size.width  = glm::clamp<unsigned int>(size.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
		size.height = glm::clamp<unsigned int>(size.height, capabilities.maxImageExtent.height, capabilities.maxImageExtent.height);
	}
	else
	{
		size = capabilities.currentExtent;
	}
	return size;
}


/**
 * Checks if the surface supports color and other required surface bits
 * If so constructs a ImageUsageFlags bitmask that is returned in outUsage
 * @return if the surface supports all the previously defined bits
 */
bool getImageUsage(const VkSurfaceCapabilitiesKHR& capabilities, VkImageUsageFlags& outUsage)
{
	const std::vector<VkImageUsageFlags>& desir_usages = getRequestedImageUsages();
	assert(desir_usages.size() > 0);

	// Needs to be always present
	outUsage = desir_usages[0];

	for (const auto& desired_usage : desir_usages)
	{
		VkImageUsageFlags image_usage = desired_usage & capabilities.supportedUsageFlags;
		if (image_usage != desired_usage)
		{
			std::cout << "unsupported image usage flag: " << desired_usage << "\n";
			return false;
		}

		// Add bit if found as supported color
		outUsage = (outUsage | desired_usage);
	}

	return true;
}


/**
 * @return transform based on global declared above, current transform if that transform isn't available
 */
VkSurfaceTransformFlagBitsKHR getTransform(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.supportedTransforms & gTransform)
		return gTransform;
	std::cout << "unsupported surface transform: " << gTransform;
	return capabilities.currentTransform;
}


/**
 * @return the most appropriate color space based on the globals provided above
 */
bool getFormat(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& outFormat)
{
	unsigned int count(0);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr) != VK_SUCCESS)
	{
		std::cout << "unable to query number of supported surface formats";
		return false;
	}

	std::vector<VkSurfaceFormatKHR> found_formats(count);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, found_formats.data()) != VK_SUCCESS)
	{
		std::cout << "unable to query all supported surface formats\n";
		return false;
	}

	// This means there are no restrictions on the supported format.
	// Preference would work
	if (found_formats.size() == 1 && found_formats[0].format == VK_FORMAT_UNDEFINED)
	{
		outFormat.format = gFormat;
		outFormat.colorSpace = gColorSpace;
		return true;
	}

	// Otherwise check if both are supported
	for (const auto& found_format_outer : found_formats)
	{
		// Format found
		if (found_format_outer.format == gFormat)
		{
			outFormat.format = found_format_outer.format;
			for (const auto& found_format_inner : found_formats)
			{
				// Color space found
				if (found_format_inner.colorSpace == gColorSpace)
				{
					outFormat.colorSpace = found_format_inner.colorSpace;
					return true;
				}
			}

			// No matching color space, pick first one
			std::cout << "warning: no matching color space found, picking first available one\n!";
			outFormat.colorSpace = found_formats[0].colorSpace;
			return true;
		}
	}

	// No matching formats found
	std::cout << "warning: no matching color format found, picking first available one\n";
	outFormat = found_formats[0];
	return true;
}


/**
 * creates the swap chain using utility functions above to retrieve swap chain properties
 * Swap chain is associated with a single window (surface) and allows us to display images to screen
 */
bool createSwapChain(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device, VkSwapchainKHR& outSwapChain)
{
	// Get properties of surface, necessary for creation of swap-chain
	VkSurfaceCapabilitiesKHR surface_properties;
	if (!getSurfaceProperties(physicalDevice, surface, surface_properties))
		return false;

	// Get the image presentation mode (synced, immediate etc.)
	VkPresentModeKHR presentation_mode = gPresentationMode;
	if (!getPresentationMode(surface, physicalDevice, presentation_mode))
		return false;

	// Get other swap chain related features
	unsigned int swap_image_count = getNumberOfSwapImages(surface_properties);
	
	// Size of the images
	VkExtent2D swap_image_extent = getSwapImageSize(surface_properties);
	
	// Get image usage (color etc.)
	VkImageUsageFlags usage_flags;
	if (!getImageUsage(surface_properties, usage_flags))
		return false;

	// Get the transform, falls back on current transform when transform is not supported
	VkSurfaceTransformFlagBitsKHR transform = getTransform(surface_properties);

	// Get swapchain image format
	VkSurfaceFormatKHR image_format;
	if (!getFormat(physicalDevice, surface, image_format))
		return false;

	// Old swap chain
	VkSwapchainKHR old_swap_chain = outSwapChain;

	// Populate swapchain creation info
	VkSwapchainCreateInfoKHR swap_info;
	swap_info.pNext = nullptr;
	swap_info.flags = 0;
	swap_info.surface = surface;
	swap_info.minImageCount = swap_image_count;
	swap_info.imageFormat = image_format.format;
	swap_info.imageColorSpace = image_format.colorSpace;
	swap_info.imageExtent = swap_image_extent;
	swap_info.imageArrayLayers = 1;
	swap_info.imageUsage = usage_flags;
	swap_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swap_info.queueFamilyIndexCount = 0;
	swap_info.pQueueFamilyIndices = nullptr;
	swap_info.preTransform = transform;
	swap_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_info.presentMode = presentation_mode;
	swap_info.clipped = true;
	swap_info.oldSwapchain = NULL;
	swap_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

	// Destroy old swap chain
	if (old_swap_chain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device, old_swap_chain, nullptr);
		old_swap_chain = VK_NULL_HANDLE;
	}

	// Create new one
	if (vkCreateSwapchainKHR(device, &swap_info, nullptr, &old_swap_chain) != VK_SUCCESS)
	{
		std::cout << "unable to create swap chain\n";
		return false;
	}

	// Store handle
	outSwapChain = old_swap_chain;
	return true;
}


/**
 *	Returns the handles of all the images in a swap chain, result is stored in outImageHandles
 */
bool getSwapChainImageHandles(VkDevice device, VkSwapchainKHR chain, std::vector<VkImage>& outImageHandles)
{
	unsigned int image_count(0);
	VkResult res = vkGetSwapchainImagesKHR(device, chain, &image_count, nullptr);
	if (res != VK_SUCCESS)
	{
		std::cout << "unable to get number of images in swap chain\n";
		return false;
	}

	outImageHandles.clear();
	outImageHandles.resize(image_count);
	if (vkGetSwapchainImagesKHR(device, chain, &image_count, outImageHandles.data()) != VK_SUCCESS)
	{
		std::cout << "unable to get image handles from swap chain\n";
		return false;
	}
	return true;
}


/**
 * Create a vulkan window
 */
SDL_Window* createWindow()
{
	return SDL_CreateWindow (
		gAppName,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		gWindowWidth,
		gWindowHeight,
		SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
	);
}


/**
 *	Destroys the vulkan instance
 */
void quit(VkInstance instance, VkDevice device, VkDebugReportCallbackEXT callback, VkSwapchainKHR chain)
{
	SDL_Quit();
	vkDestroySwapchainKHR(device, chain, nullptr);
	vkDestroyDevice(device, nullptr);
	destroyDebugReportCallbackEXT(instance, callback, nullptr);
	vkDestroyInstance(instance, nullptr);
}


int main(int argc, char *argv[])
{
	// Initialize SDL
	if (!initSDL())
		return -1;

	// Create vulkan compatible window
	SDL_Window* window = createWindow();

	printf ( "%s\n", SDL_GetError ());

	// Get available vulkan extensions, necessary for interfacing with native window
	// SDL takes care of this call and returns, next to the default VK_KHR_surface a platform specific extension
	// When initializing the vulkan instance these extensions have to be enabled in order to create a valid
	// surface later on.
	std::vector<std::string> found_extensions;
	if (!getAvailableVulkanExtensions(window, found_extensions))
		return -1;

	// Get available vulkan layer extensions, notify when not all could be found
	std::vector<std::string> found_layers;
	if (!getAvailableVulkanLayers(found_layers))
		return -1;

	// Warn when not all requested layers could be found
	if (found_layers.size() != getRequestedLayerNames().size())
		std::cout << "warning! not all requested layers could be found!\n";

	// Create Vulkan Instance
	VkInstance instance;
	if (!createVulkanInstance(found_layers, found_extensions, instance))
		return -1;

	// Vulkan messaging callback
	VkDebugReportCallbackEXT callback;
	setupDebugCallback(instance, callback);

	// Select GPU after succsessful creation of a vulkan instance (jeeeej no global states anymore)
	VkPhysicalDevice gpu;
	unsigned int graphics_queue_index(-1);
	if (!selectGPU(instance, gpu, graphics_queue_index))
		return -1;

	// Create a logical device that interfaces with the physical device
	VkDevice device;
	if (!createLogicalDevice(gpu, graphics_queue_index, found_layers, device))
		return -1;

	// Create the surface we want to render to, associated with the window we created before
	// This call also checks if the created surface is compatible with the previously selected physical device and associated render queue
	VkSurfaceKHR presentation_surface;
	if (!createSurface(window, instance, gpu, graphics_queue_index, presentation_surface))
		return -1;

	// Create swap chain
	VkSwapchainKHR swap_chain = NULL;
	if (!createSwapChain(presentation_surface, gpu, device, swap_chain))
		return -1;

	// Get image handles from swap chain
	std::vector<VkImage> chain_images;
	if (!getSwapChainImageHandles(device, swap_chain, chain_images))
		return -1;

	// Fetch the queue we want to submit the actual commands to
	VkQueue graphics_queue;
	getDeviceQueue(device, graphics_queue_index, graphics_queue);

	std::cout << "\nsuccessfully initialized vulkan and physical device (gpu).\n";
	std::cout << "successfully created a window and compatible surface\n";
	std::cout << "successfully created swapchain\n";
	std::cout << "ready to render!\n";

	// WOOP, finally ready to render some stuff!
	bool run = true;
	while (run)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				run = false;
			}
		}
	}

	// Destroy Vulkan Instance
	quit(instance, device, callback, swap_chain);
	
	return 1;
}
